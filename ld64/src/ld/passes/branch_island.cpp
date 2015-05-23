/* -*- mode: C++; c-basic-offset: 4; tab-width: 4 -*-
 *
 * Copyright (c) 2009 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <dlfcn.h>
#include <libkern/OSByteOrder.h>

#include <vector>
#include <map>

#include "MachOFileAbstraction.hpp"
#include "ld.hpp"
#include "branch_island.h"

namespace ld {
namespace passes {
namespace branch_island {

static bool seenCrossSectBr;
static bool seenThumbBr;
static uint64_t lowestTextAddr;
static uint64_t furthestStubSect;
static uint64_t furthestCodeOrStubSect;
static uint64_t sizeOfTEXTSeg;
static unsigned sectionsWithBanches;

static std::map<const Atom*, uint64_t> sAtomToAddress;

struct TargetAndOffset { const ld::Atom* atom; uint32_t offset; };
class TargetAndOffsetComparor
{
public:
	bool operator()(const TargetAndOffset& left, const TargetAndOffset& right) const
	{
		if ( left.atom != right.atom )
			return ( left.atom < right.atom );
		return ( left.offset < right.offset );
	}
};


static bool _s_log = false;
//static ld::Section _s_text_section("__TEXT", "__text", ld::Section::typeCode);

class PPCBranchIslandAtom : public ld::Atom {
public:
	PPCBranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland,
							ld::Atom::symbolTableIn, false, false, false, ld::Atom::Alignment(2)), _name(nm)
				 {
				    _fxups.clear();
					if (finalTarget.offset == 0)
						hasAddend = false;
					else
						hasAddend = true;
					// We are doing the final branch, which might need the addend from the original fixups.
					if ((target == finalTarget.atom) && hasAddend) {
						_fxups.push_back(Fixup(0, ld::Fixup::k1of3, ld::Fixup::kindSetTargetAddress, target));
						_fxups.push_back(Fixup(0, ld::Fixup::k2of3, ld::Fixup::kindAddAddend, finalTarget.offset));
						_fxups.push_back(Fixup(0, ld::Fixup::k3of3, ld::Fixup::kindStorePPCBranch24));
					} else {
						_fxups.push_back(Fixup(0, ld::Fixup::k1of1, ld::Fixup::kindStoreTargetAddressPPCBranch24, target));
					}
					// Record a shorthand version of the final destination, so that OutputFile can optimize the
					// intermediate islands away where the final target turns out to be reachable.
					if (hasAddend) {
						_fxups.push_back(Fixup(0, ld::Fixup::k1of2, ld::Fixup::kindIslandTarget, finalTarget.atom));
						_fxups.push_back(Fixup(0, ld::Fixup::k2of2, ld::Fixup::kindAddAddend, finalTarget.offset));
					} else {
						_fxups.push_back(Fixup(0, ld::Fixup::k1of1, ld::Fixup::kindIslandTarget, finalTarget.atom));
					}
					if (_s_log) fprintf(stderr, "  %s: PPC jump instruction branch island to %s (final target %s%s) %d fixups\n",
										nm, target->name(), finalTarget.atom->name(), (hasAddend?" has addend":""),
										_fxups.size());
				}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual bool							translationUnitSource(const char** dir, const char**) const
																			{ return false; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 4; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		OSWriteBigInt32(buffer, 0, 0x48000000);
	}
	virtual void							setScope(Scope)					{ }
	virtual ld::Fixup::iterator				fixupsBegin()  const			{ return (ld::Fixup*)&_fxups[0]; }
	virtual ld::Fixup::iterator				fixupsEnd()	 const				{ return (ld::Fixup*)&_fxups[_fxups.size()]; }

private:
	const char*								_name;
	mutable std::vector<ld::Fixup>			_fxups;
	bool									hasAddend;
};

#if SUPPORT_ARCH_arm64

class ARM64BranchIslandAtom : public ld::Atom {
public:
											ARM64BranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, false, false, ld::Atom::Alignment(2)), 
				_name(nm),
				_fixup1(0, ld::Fixup::k1of1, ld::Fixup::kindStoreTargetAddressARM64Branch26, target),
				_fixup2(0, ld::Fixup::k1of1, ld::Fixup::kindIslandTarget, finalTarget.atom) {
					if (_s_log) fprintf(stderr, "%p: ARM64 branch island to final target %s\n",
										this, finalTarget.atom->name());
				}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 4; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		OSWriteLittleInt32(buffer, 0, 0x14000000);
	}
	virtual void							setScope(Scope)					{ }
	virtual ld::Fixup::iterator				fixupsBegin() const				{ return (ld::Fixup*)&_fixup1; }
	virtual ld::Fixup::iterator				fixupsEnd()	const 				{ return &((ld::Fixup*)&_fixup2)[1]; }

private:
	const char*								_name;
	ld::Fixup								_fixup1;
	ld::Fixup								_fixup2;
};
#endif


class ARMtoARMBranchIslandAtom : public ld::Atom {
public:
	ARMtoARMBranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, false, false, ld::Atom::Alignment(2)), 
				_name(nm),
				_fixup1(0, ld::Fixup::k1of1, ld::Fixup::kindStoreTargetAddressARMBranch24, target),
				_fixup2(0, ld::Fixup::k1of1, ld::Fixup::kindIslandTarget, finalTarget.atom) {
					if (_s_log) fprintf(stderr, "%p: ARM-to-ARM branch island to final target %s\n", 
										this, finalTarget.atom->name());
				}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 4; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		OSWriteLittleInt32(buffer, 0, 0xEA000000);
	}
	virtual void							setScope(Scope)					{ }
	virtual ld::Fixup::iterator				fixupsBegin() const				{ return (ld::Fixup*)&_fixup1; }
	virtual ld::Fixup::iterator				fixupsEnd()	const 				{ return &((ld::Fixup*)&_fixup2)[1]; }

private:
	const char*								_name;
	ld::Fixup								_fixup1;
	ld::Fixup								_fixup2;
};



class ARMtoThumb1BranchIslandAtom : public ld::Atom {
public:
	ARMtoThumb1BranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, false, false, ld::Atom::Alignment(2)), 
				_name(nm),
				_finalTarget(finalTarget) {
					if (_s_log) fprintf(stderr, "%p: ARM-to-thumb1 branch island to final target %s\n", 
										this, finalTarget.atom->name());
				}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 16; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		// There is no large displacement thumb1 branch instruction.
		// Instead use ARM instructions that can jump to thumb.
		// we use a 32-bit displacement, so we can directly jump to target which means no island hopping
		int64_t displacement = _finalTarget.atom->finalAddress() + _finalTarget.offset - (this->finalAddress() + 12);
		if ( _finalTarget.atom->isThumb() )
			displacement |= 1;
		OSWriteLittleInt32(&buffer[ 0], 0, 0xe59fc004);	// 	ldr  ip, pc + 4
		OSWriteLittleInt32(&buffer[ 4], 0, 0xe08fc00c);	// 	add	 ip, pc, ip
		OSWriteLittleInt32(&buffer[ 8], 0, 0xe12fff1c);	// 	bx	 ip
		OSWriteLittleInt32(&buffer[12], 0, displacement);	// 	.long target-this		
	}
	virtual void							setScope(Scope)					{ }

private:
	const char*								_name;
	TargetAndOffset							_finalTarget;
};



class Thumb2toThumbBranchIslandAtom : public ld::Atom {
public:
	Thumb2toThumbBranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, true, false, ld::Atom::Alignment(1)), 
				_name(nm),
				_fixup1(0, ld::Fixup::k1of1, ld::Fixup::kindStoreTargetAddressThumbBranch22, target),
				_fixup2(0, ld::Fixup::k1of1, ld::Fixup::kindIslandTarget, finalTarget.atom)		{ 
					if (_s_log) fprintf(stderr, "%p: Thumb-to-thumb branch island to final target %s\n", 
										this, finalTarget.atom->name());
					}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 4; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		OSWriteLittleInt32(buffer, 0, 0xf0008000);
	}
	virtual void							setScope(Scope)					{ }
	virtual ld::Fixup::iterator				fixupsBegin() const				{ return (ld::Fixup*)&_fixup1; }
	virtual ld::Fixup::iterator				fixupsEnd()	const 				{ return &((ld::Fixup*)&_fixup2)[1]; }

private:
	const char*								_name;
	ld::Fixup								_fixup1;
	ld::Fixup								_fixup2;
};



class Thumb2toThumbBranchAbsoluteIslandAtom : public ld::Atom {
public:
	Thumb2toThumbBranchAbsoluteIslandAtom(const char* nm, const ld::Section& inSect, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, true, false, ld::Atom::Alignment(1)), 
				_name(nm),
				_fixup1(0, ld::Fixup::k1of2, ld::Fixup::kindSetTargetAddress, finalTarget.atom),
				_fixup2(0, ld::Fixup::k2of2, ld::Fixup::kindStoreThumbLow16),
				_fixup3(4, ld::Fixup::k1of2, ld::Fixup::kindSetTargetAddress, finalTarget.atom),
				_fixup4(4, ld::Fixup::k2of2, ld::Fixup::kindStoreThumbHigh16),
				_fixup5(0, ld::Fixup::k1of1, ld::Fixup::kindIslandTarget, finalTarget.atom) {
					if (_s_log) fprintf(stderr, "%p: Thumb-to-thumb absolute branch island to final target %s\n", 
										this, finalTarget.atom->name());
					}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 10; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		OSWriteLittleInt32(&buffer[0], 0, 0x0c00f240);	// 	movw	r12, #0x5678
		OSWriteLittleInt32(&buffer[4], 0, 0x0c00f2c0);	// 	movt	r12, #0x1234
		OSWriteLittleInt16(&buffer[8], 0, 0x4760);		// 	bx		r12
	}
	virtual void							setScope(Scope)					{ }
	virtual ld::Fixup::iterator				fixupsBegin() const				{ return (ld::Fixup*)&_fixup1; }
	virtual ld::Fixup::iterator				fixupsEnd()	const 				{ return &((ld::Fixup*)&_fixup5)[1]; }

private:
	const char*								_name;
	ld::Fixup								_fixup1;
	ld::Fixup								_fixup2;
	ld::Fixup								_fixup3;
	ld::Fixup								_fixup4;
	ld::Fixup								_fixup5;
};



class NoPicARMtoThumbMBranchIslandAtom : public ld::Atom {
public:
	NoPicARMtoThumbMBranchIslandAtom(const char* nm, const ld::Section& inSect, const ld::Atom* target, TargetAndOffset finalTarget)
				: ld::Atom(inSect, ld::Atom::definitionRegular, ld::Atom::combineNever,
							ld::Atom::scopeLinkageUnit, ld::Atom::typeBranchIsland, 
							ld::Atom::symbolTableIn, false, false, false, ld::Atom::Alignment(2)), 
				_name(nm),
				_finalTarget(finalTarget) {
					if (_s_log) fprintf(stderr, "%p: NoPIC ARM-to-Thumb branch island to final target %s\n", 
										this, finalTarget.atom->name());
					}

	virtual const ld::File*					file() const					{ return NULL; }
	virtual const char*						name() const					{ return _name; }
	virtual uint64_t						size() const					{ return 8; }
	virtual uint64_t						objectAddress() const			{ return 0; }
	virtual void							copyRawContent(uint8_t buffer[]) const {
		// There is no large displacement thumb1 branch instruction.
		// Instead use ARM instructions that can jump to thumb.
		// we use a 32-bit displacement, so we can directly jump to final target which means no island hopping
		uint32_t targetAddr = _finalTarget.atom->finalAddress();
		if ( _finalTarget.atom->isThumb() )
			targetAddr |= 1;
		OSWriteLittleInt32(&buffer[0], 0, 0xe51ff004);	// 	ldr	pc, [pc, #-4]
		OSWriteLittleInt32(&buffer[4], 0, targetAddr);	// 	.long target-this		
	}
	virtual void							setScope(Scope)					{ }

private:
	const char*								_name;
	TargetAndOffset							_finalTarget;
};


static ld::Atom* makeBranchIsland(const Options& opts, ld::Fixup::Kind kind, int islandRegion, const ld::Atom* nextTarget, 
									TargetAndOffset finalTarget, const ld::Section& inSect, bool crossSectionBranch)
{
	char *name = NULL;
	const char *aname;
	if (finalTarget.atom->name() == NULL || finalTarget.atom->name()[0] == 0)
	  aname = "anon";
	else
	  aname = finalTarget.atom->name();
	if ( finalTarget.offset == 0 ) {
		if ( islandRegion == 0 )
			asprintf(&name, "%s.island", aname);
		else
			asprintf(&name, "%s.island.%d", aname, islandRegion);
	}
	else {
		asprintf(&name, "%s_plus_%d.island.%d", aname, finalTarget.offset, islandRegion);
	}

	switch ( kind ) {
		case ld::Fixup::kindStorePPCBranch24:
		case ld::Fixup::kindStoreTargetAddressPPCBranch24:
			return new PPCBranchIslandAtom(name, inSect, nextTarget, finalTarget);
			break;
		case ld::Fixup::kindStoreARMBranch24:
		case ld::Fixup::kindStoreThumbBranch22:
		case ld::Fixup::kindStoreTargetAddressARMBranch24:
		case ld::Fixup::kindStoreTargetAddressThumbBranch22:
			if ( crossSectionBranch && opts.preferSubArchitecture() && opts.archSupportsThumb2() ) {
				return new Thumb2toThumbBranchAbsoluteIslandAtom(name, inSect, finalTarget);
			}
			else if ( finalTarget.atom->isThumb() ) {
				if ( opts.preferSubArchitecture() && opts.archSupportsThumb2() ) {
					return new Thumb2toThumbBranchIslandAtom(name, inSect, nextTarget, finalTarget);
				}
				else if ( opts.outputSlidable() ) {
					return new ARMtoThumb1BranchIslandAtom(name, inSect, nextTarget, finalTarget);
				}
				else {
					return new NoPicARMtoThumbMBranchIslandAtom(name, inSect, nextTarget, finalTarget);
				}
			}
			else {
				return new ARMtoARMBranchIslandAtom(name, inSect, nextTarget, finalTarget);
			}
			break;
#if SUPPORT_ARCH_arm64
	    case ld::Fixup::kindStoreARM64Branch26:
		case ld::Fixup::kindStoreTargetAddressARM64Branch26:
			return new ARM64BranchIslandAtom(name, inSect, nextTarget, finalTarget);
			break;
#endif
		default:
			assert(0 && "unexpected branch kind");
			break;
	}
	return NULL;
}


static uint64_t textSizeWhenMightNeedBranchIslands(const Options& opts, bool seenThumbBranch)
{
	switch ( opts.architecture() ) {
		case CPU_TYPE_POWERPC:
		case CPU_TYPE_POWERPC64:
			return 32000000;  // PPC can branch +/- 32MB
			break;
		case CPU_TYPE_ARM:
			if ( ! seenThumbBranch )
				return 32000000;  // ARM can branch +/- 32MB
			else if ( opts.preferSubArchitecture() && opts.archSupportsThumb2() ) 
				return 16000000;  // thumb2 can branch +/- 16MB
			else
				return  4000000;  // thumb1 can branch +/- 4MB
			break;
#if SUPPORT_ARCH_arm64
		case CPU_TYPE_ARM64:
			return 128000000; // arm64 can branch +/- 128MB
			break;
#endif
	}
	assert(0 && "unexpected architecture");
	return 0x100000000LL;
}


static uint64_t maxDistanceBetweenIslands(const Options& opts, bool seenThumbBranch)
{
	switch ( opts.architecture() ) {
		case CPU_TYPE_POWERPC:
		case CPU_TYPE_POWERPC64:
				return 30*1024*1024;	// 2MB of branch islands per 32MB
			break;
		case CPU_TYPE_ARM:
			if ( ! seenThumbBranch )
				return 30*1024*1024;	// 2MB of branch islands per 32MB
			else if ( opts.preferSubArchitecture() && opts.archSupportsThumb2() ) 
				return 14*1024*1024;	// 2MB of branch islands per 16MB
			else
				return 3500000;			// 0.5MB of branch islands per 4MB
			break;
#if SUPPORT_ARCH_arm64
		case CPU_TYPE_ARM64:
			return 124*1024*1024;		 // 4MB of branch islands per 128MB
			break;
#endif
	}
	assert(0 && "unexpected architecture");
	return 0x100000000LL;
}


// Branch Island Algorithm
//
// FIXME: write a comment describing the revised algorithm.

//

typedef std::map<TargetAndOffset,const ld::Atom*, TargetAndOffsetComparor> AtomToIsland;

static unsigned numberOfIslandRegions;
static std::vector<const ld::Atom*> branchIslandInsertionPoints; // atoms in the atom list after which branch islands will be inserted
static std::vector<ld::Internal::FinalSection*> branchIslandInsertionSections; // Section containing
static int kIslandRegionsCount;
static uint64_t kBetweenRegions;
static AtomToIsland* *regionsMap;
static uint64_t *regionAddresses;
static std::vector<const ld::Atom*>* *regionsIslands;
static unsigned int islandCount;

static void makeIslandsForSection(const Options& opts, ld::Internal& state,
								  ld::Internal::FinalSection* sect, unsigned stubCount)
{
	bool hasThumbBranches = sect->hasThumbBranches;
	bool haveCrossSectionBranches = sect->hasCrossSectionBranches;
	const bool preload = (opts.outputKind() == Options::kPreload);

if (_s_log) fprintf(stderr, "ld: checking section %s\n", sect->sectionName());
	
	// create islands for branches in sect that are out of range
	for (std::vector<const ld::Atom*>::iterator ait=sect->atoms.begin();
		 ait != sect->atoms.end(); ++ait) {
		const ld::Atom* atom = *ait;
		const ld::Atom* target = NULL;
		uint32_t addend = 0;
		ld::Fixup* fixupWithTarget = NULL;
		ld::Fixup* fixupWithAddend = NULL;
		int fu = 0;
//if (_s_log) fprintf(stderr, "atom : %s \n", atom->name());
		for (ld::Fixup::iterator fit = atom->fixupsBegin(), end=atom->fixupsEnd();
			 fit != end; ++fit) {
//if (_s_log) fprintf(stderr, "fu %d %s : %s : ", fu++, (fit->firstInCluster()?"is first":"not first"), (fit->lastInCluster()?"is last":"not last"));
			if ( fit->firstInCluster() ) {
				target = NULL;
				fixupWithTarget = NULL;
				fixupWithAddend = NULL;
				addend = 0;
			}
			switch ( fit->binding ) {
				case ld::Fixup::bindingNone:
				case ld::Fixup::bindingByNameUnbound:
//if (_s_log) fprintf(stderr, "Binding none / unbound " );
					break;
				case ld::Fixup::bindingByContentBound:
				case ld::Fixup::bindingDirectlyBound:
					target = fit->u.target;
					fixupWithTarget = fit;
//if (_s_log) fprintf(stderr, "      target : %s ", target->name());
					break;
				case ld::Fixup::bindingsIndirectlyBound:
					target = state.indirectBindingTable[fit->u.bindingIndex];
					fixupWithTarget = fit;
//if (_s_log) fprintf(stderr, "indir target : %s ", target->name());
					break;
			}
			bool haveBranch = false;
			switch (fit->kind) {
				case ld::Fixup::kindAddAddend:
					addend = fit->u.addend;
					fixupWithAddend = fit;
//if (_s_log) fprintf(stderr, " addend : %u\n", addend);
					break;
				case ld::Fixup::kindStoreTargetAddressPPCBranch24:
//if (_s_log) fprintf(stderr, " StoreTargetAddress", addend);
				case ld::Fixup::kindStorePPCBranch24:
//if (_s_log) fprintf(stderr, "  branch\n", addend);
				case ld::Fixup::kindStoreARMBranch24:
				case ld::Fixup::kindStoreThumbBranch22:
				case ld::Fixup::kindStoreTargetAddressARMBranch24:
				case ld::Fixup::kindStoreTargetAddressThumbBranch22:
#if SUPPORT_ARCH_arm64
				case ld::Fixup::kindStoreARM64Branch26:
				case ld::Fixup::kindStoreTargetAddressARM64Branch26:
#endif
					haveBranch = true;
					break;
                default:
//if (_s_log)  fprintf(stderr, " [%u] def\n", fit->kind);
                   break;
			}
			if ( haveBranch ) {
				bool crossSectionBranch = ( /*preload && */(atom->section() != target->section()) );
				int64_t srcAddr = atom->sectionOffset() + fit->offsetInAtom;
				int64_t dstAddr = target->sectionOffset() + addend;
				if ( seenCrossSectBr || preload ) {
					srcAddr = sAtomToAddress[atom] + fit->offsetInAtom;
					dstAddr = sAtomToAddress[target] + addend;
				}
				if ( target->section().type() == ld::Section::typeStub )
					dstAddr = furthestStubSect;
				int64_t displacement = dstAddr - srcAddr;
if (_s_log && (abs(displacement) > kBetweenRegions) ) fprintf(stderr, "from %s to %s delta : 0x%0" PRIx64 " in section %s\n",
					atom->name(), target->name(), displacement, sect->sectionName());
				TargetAndOffset finalTargetAndOffset = { target, addend };
				const int64_t kBranchLimit = kBetweenRegions;
				if ( crossSectionBranch && preload && ((displacement > kBranchLimit) || (displacement < (-kBranchLimit))) ) {
					const ld::Atom* island;
					AtomToIsland* region = regionsMap[0];
					AtomToIsland::iterator pos = region->find(finalTargetAndOffset);
					if ( pos == region->end() ) {
						island = makeBranchIsland(opts, fit->kind, 0, target, finalTargetAndOffset, atom->section(), true);
						(*region)[finalTargetAndOffset] = island;
						if (_s_log) fprintf(stderr, "added absolute branching island %p %s, displacement=%lld\n", 
												island, island->name(), displacement);
						++islandCount;
						regionsIslands[0]->push_back(island);
						state.atomToSection[island] = sect;
					}
					else {
						island = pos->second;
					}
					if (_s_log) fprintf(stderr, "using island %p %s for branch to %s from %s\n", island, island->name(), target->name(), atom->name());
					fixupWithTarget->u.target = island;
					fixupWithTarget->binding = ld::Fixup::bindingDirectlyBound;
					if (fixupWithAddend)
						fixupWithAddend->u.addend = 0;
				}
				else if ( displacement > kBranchLimit ) {
					// create forward branch chain
					const ld::Atom* nextTarget = target;
					if (_s_log) fprintf(stderr, "  +need forward branching island srcAdr=0x%08llX, dstAdr=0x%08llX, target=%s\n",
														srcAddr, dstAddr, target->name());
					for (int i=kIslandRegionsCount-1; i >=0 ; --i) {
						AtomToIsland* region = regionsMap[i];
						int64_t islandRegionAddr = regionAddresses[i];
						if ( (srcAddr < islandRegionAddr) && ((islandRegionAddr <= dstAddr)) ) { 
							AtomToIsland::iterator pos = region->find(finalTargetAndOffset);
							if ( pos == region->end() ) {
								ld::Atom* island = makeBranchIsland(opts, fit->kind, i, nextTarget, finalTargetAndOffset, *branchIslandInsertionSections[i], false);
								(*region)[finalTargetAndOffset] = island;
								if (_s_log) fprintf(stderr, "  +added forward branching island %p %s to region %d (in section %s) for %s\n", island, island->name(), i, branchIslandInsertionSections[i]->sectionName(), atom->name());
								regionsIslands[i]->push_back(island);
								state.atomToSection[island] = sect;
								++islandCount;
								nextTarget = island;
							}
							else {
								nextTarget = pos->second;
							}
						}
					}
					if (_s_log) fprintf(stderr, "  +using island %p %s for branch to %s from %s\n", nextTarget, nextTarget->name(), target->name(), atom->name());
					fixupWithTarget->u.target = nextTarget;
					fixupWithTarget->binding = ld::Fixup::bindingDirectlyBound;
					if (fixupWithAddend)
						fixupWithAddend->u.addend = 0;
				}
				else if ( displacement < (-kBranchLimit) ) {
					// create back branching chain
					const ld::Atom* prevTarget = target;
					for (int i=0; i < kIslandRegionsCount ; ++i) {
						AtomToIsland* region = regionsMap[i];
						int64_t islandRegionAddr = regionAddresses[i];
						if ( (dstAddr < islandRegionAddr) && (islandRegionAddr <= srcAddr) ) {
							if (_s_log) fprintf(stderr, "  -need backward branching island srcAdr=0x%08llX, dstAdr=0x%08llX, target=%s\n", srcAddr, dstAddr, target->name());
							AtomToIsland::iterator pos = region->find(finalTargetAndOffset);
							if ( pos == region->end() ) {
								ld::Atom* island = makeBranchIsland(opts, fit->kind, i, prevTarget, finalTargetAndOffset, *branchIslandInsertionSections[i], false);
								(*region)[finalTargetAndOffset] = island;
								if (_s_log) fprintf(stderr, "  -added back branching island %p %s to region %d (in section %s) for %s\n", island, island->name(), i, branchIslandInsertionSections[i]->sectionName(), atom->name());
								regionsIslands[i]->push_back(island);
								state.atomToSection[island] = sect;
								++islandCount;
								prevTarget = island;
							}
							else {
								prevTarget = pos->second;
							}
						}
					}
					if (_s_log) fprintf(stderr, "  -using back island %p %s for %s\n", prevTarget, prevTarget->name(), atom->name());
					fixupWithTarget->u.target = prevTarget;
					fixupWithTarget->binding = ld::Fixup::bindingDirectlyBound;
					if (fixupWithAddend)
						fixupWithAddend->u.addend = 0;
				}
			}
		}
	}
}

// Layout the atoms well enough to determine where to insert
static void buildAddressMap(const Options& opts, ld::Internal& state) {
	
	// Assign addresses to atoms in a side table
	const bool log = false;
	if ( log ) fprintf(stderr, "buildAddressMap()\n");
	for (std::vector<ld::Internal::FinalSection*>::iterator sit = state.sections.begin();
	     sit != state.sections.end(); ++sit) {
		ld::Internal::FinalSection* sect = *sit;
		uint16_t maxAlignment = 0;
		uint64_t offset = 0;
		if ( log ) fprintf(stderr, "  section=%s/%s, address=0x%08llX\n",
		                   sect->segmentName(), sect->sectionName(), sect->address);
		for (std::vector<const ld::Atom*>::iterator ait = sect->atoms.begin();
		     ait != sect->atoms.end(); ++ait) {
			const ld::Atom* atom = *ait;
			uint32_t atomAlignmentPowerOf2 = atom->alignment().powerOf2;
			uint32_t atomModulus = atom->alignment().modulus;
			if ( atomAlignmentPowerOf2 > maxAlignment )
				maxAlignment = atomAlignmentPowerOf2;
			// calculate section offset for this atom
			uint64_t alignment = 1 << atomAlignmentPowerOf2;
			uint64_t currentModulus = (offset % alignment);
			uint64_t requiredModulus = atomModulus;
			if ( currentModulus != requiredModulus ) {
				if ( requiredModulus > currentModulus )
					offset += requiredModulus-currentModulus;
				else
					offset += requiredModulus+alignment-currentModulus;
			}
			
			if ( log ) fprintf(stderr, "    0x%08llX atom=%p, name=%s\n", sect->address+offset, atom, atom->name());
			sAtomToAddress[atom] = sect->address + offset;
			
			offset += atom->size();
		}
	}
}

// Initial (conservative) check as to whether islands might be required.
// If the total size of sections containing code exceeds that reachable by a
// branch instruction (including ability to reach a stubs section), then we
// assume conservatively that branch islands could be needed.  We assume that
// the lowest address that might need to reach somewhere is 0.
static bool mightNeedBranchIslands(const Options& opts, ld::Internal& state) {
    bool anySectNeedsIslands = false;
	furthestStubSect = 0;
	furthestCodeOrStubSect = 0;
	sizeOfTEXTSeg = 0;
	sectionsWithBanches = 0;

	// Assign addresses to sections
	state.setSectionSizesAndAlignments();
	state.assignFileOffsets();

	for (std::vector<ld::Internal::FinalSection*>::iterator sit = state.sections.begin();
	     sit != state.sections.end(); ++sit) {
		ld::Internal::FinalSection* sect = *sit;
		if (strncmp(sect->segmentName(), "__TEXT", 6) == 0) {
		  lowestTextAddr = lowestTextAddr > sect->address ? sect->address : lowestTextAddr;
		  sizeOfTEXTSeg += sect->size;
		}
		// To a first approximation, the longest distance is from 0 to the end of the
		// section we're currently looking at.
		if ( sect->type() == ld::Section::typeStub) {
			furthestStubSect = sect->address + sect->size;
			furthestCodeOrStubSect = sect->address + sect->size;
		} else if (sect->type() == ld::Section::typeCode) {
			// So now see whether we have branches, cross-section branches and/or
			// thumb ones.
			furthestCodeOrStubSect = sect->address + sect->size;
			for (std::vector<const ld::Atom*>::iterator ait=sect->atoms.begin();
				 ait != sect->atoms.end(); ++ait) {
				const ld::Atom* atom = *ait;
				// check for thumb branches and cross section branches
				const ld::Atom* target = NULL;
				for (ld::Fixup::iterator fit = atom->fixupsBegin(), end=atom->fixupsEnd();
					 fit != end; ++fit) {
					if ( fit->firstInCluster() ) {
						target = NULL;
					}
					switch ( fit->binding ) {
					case ld::Fixup::bindingNone:
					case ld::Fixup::bindingByNameUnbound:
						break;
					case ld::Fixup::bindingByContentBound:
					case ld::Fixup::bindingDirectlyBound:
						target = fit->u.target;
						break;
					case ld::Fixup::bindingsIndirectlyBound:
						target = state.indirectBindingTable[fit->u.bindingIndex];
						break;
					}
					bool haveBranch = false;
					switch (fit->kind) {
					case ld::Fixup::kindStoreThumbBranch22:
					case ld::Fixup::kindStoreTargetAddressThumbBranch22:
						sect->hasThumbBranches = seenThumbBr = true;
						// fall into arm branch case
					case ld::Fixup::kindStoreARMBranch24:
					case ld::Fixup::kindStoreTargetAddressARMBranch24:
					case ld::Fixup::kindStorePPCBranch24:
					case ld::Fixup::kindStoreTargetAddressPPCBranch24:
						sect->hasBranches = true;
						haveBranch = true;
						break;
					default:
						break;
					}
					// We will count branches to stubs as cross-section since we
					// don't know what funky User-specific sections might get made.
					if ( haveBranch && (atom->section() != target->section()) )
						sect->hasCrossSectionBranches = seenCrossSectBr = true;
				} // for each fixup.
			} // for each atom.
			// So if we have branches, see if the section needs islands (on its own)
			if ( sect->hasBranches &&
				(sect->size > textSizeWhenMightNeedBranchIslands(opts, sect->hasThumbBranches)) )
				sect->needsIslands = anySectNeedsIslands = true;
			if (sect->hasBranches)
				sectionsWithBanches++;
		} // in code sections.
	} // for each section.
	// If the codesize > smallest reachable and there are inter-section branches, assume that
	// we need islands.
	if (((furthestCodeOrStubSect-lowestTextAddr) > textSizeWhenMightNeedBranchIslands(opts, seenThumbBr))
	     && seenCrossSectBr)
		anySectNeedsIslands = true;
if (_s_log)
fprintf (stderr, "TEXT seg size %" PRIu64 "M lowest Addr 0x%08" PRIx64 " furthest stub 0x%08" PRIx64 " furthest code or stub 0x%08" PRIx64 " %s islands\n",
		 sizeOfTEXTSeg/(1024*1024), lowestTextAddr, furthestStubSect, furthestCodeOrStubSect, (anySectNeedsIslands? "needs" : "no"));
	kBetweenRegions = maxDistanceBetweenIslands(opts, seenThumbBr);
	return anySectNeedsIslands;
}

// Figure out how many regions of branch islands will be needed, and their locations.
// Construct a vector containing the atoms after which branch islands will be inserted,
// taking into account follow on fixups. No atom run without an island can exceed kBetweenRegions.
void findIslandInsertionPoints(const Options& opts, ld::Internal& state) {

	uint64_t previousIslandEndAddr = lowestTextAddr;
	branchIslandInsertionPoints.reserve(furthestCodeOrStubSect/kBetweenRegions*2);

	for (std::vector<ld::Internal::FinalSection*>::iterator sit = state.sections.begin();
	     sit != state.sections.end(); ++sit) {
		ld::Internal::FinalSection* sect = *sit;
		const ld::Atom *insertionPoint = NULL;
		if ( previousIslandEndAddr + kBetweenRegions > sect->address + sect->size )
			continue; // No Islands in this section.

		if ( previousIslandEndAddr + kBetweenRegions > furthestCodeOrStubSect )
			break; // Done.

		if (sect->type() != ld::Section::typeCode) {
fprintf(stderr, "**Want to insert branch island into non-code section %s/%s, wanted this address=0x%08" PRIu64 "\n",
		sect->segmentName(), sect->sectionName(), previousIslandEndAddr + kBetweenRegions);
		}

		// We expect one of more islands in this section.
		for (std::vector<const ld::Atom*>::iterator it=sect->atoms.begin();
			 it != sect->atoms.end(); it++) {
			const ld::Atom* atom = *it;
			// if we move past the next atom, will the run length exceed kBetweenRegions?
			if ( sect->address + atom->sectionOffset() + atom->size()
			     > previousIslandEndAddr + kBetweenRegions ) {
				// yes. Add the last known good location (atom) for inserting a branch island.
				if ( insertionPoint == NULL )
					throwf("Unable to insert branch island. No insertion point available.");
				branchIslandInsertionPoints.push_back(insertionPoint);
				branchIslandInsertionSections.push_back(sect);
				previousIslandEndAddr = sect->address + insertionPoint->sectionOffset() + insertionPoint->size();
				insertionPoint = NULL;
			}
			// Can we insert an island after this atom? If so then keep track of it.
			if ( !atom->hasFixupsOfKind(ld::Fixup::kindNoneFollowOn) )
				insertionPoint = atom;
		}
	} // until we pass the endpoint.

	kIslandRegionsCount = branchIslandInsertionPoints.size();
	if (_s_log) fprintf(stderr, "ld: will use %u branch island regions\n", kIslandRegionsCount);

	typedef AtomToIsland* AtomToIsland_p;
    regionsMap = new AtomToIsland_p[kIslandRegionsCount];
	regionAddresses = new uint64_t[kIslandRegionsCount];
	typedef std::vector<const ld::Atom*>* region_p;
	regionsIslands = new region_p[kIslandRegionsCount];
	for(int i=0; i < kIslandRegionsCount; ++i) {
		const ld::Internal::FinalSection* sect = branchIslandInsertionSections[i];
		regionsMap[i] = new AtomToIsland();
		regionsIslands[i] = new std::vector<const ld::Atom*>();
		regionAddresses[i] = branchIslandInsertionPoints[i]->sectionOffset() + branchIslandInsertionPoints[i]->size();
		if (_s_log) fprintf(stderr, "ld: branch islands will be inserted at 0x%08llX after %s in section %s/%s\n",
		 regionAddresses[i], branchIslandInsertionPoints[i]->name(), sect->segmentName(), sect->sectionName());
	}
}

void doPass(const Options& opts, ld::Internal& state)
{	
	// only make branch islands in final linked images
	if ( opts.outputKind() == Options::kObjectFile )
		return;
	
	// Allow user to disable branch island generation
	if ( !opts.allowBranchIslands() )
		return;

	// only ARM[64] and PPC need branch islands
	switch ( opts.architecture() ) {
		case CPU_TYPE_POWERPC:
		case CPU_TYPE_POWERPC64:
		case CPU_TYPE_ARM:
#if SUPPORT_ARCH_arm64
		case CPU_TYPE_ARM64:
#endif
			break;
		default:
			return;
	}

	seenCrossSectBr = false;
	seenThumbBr = false;
	lowestTextAddr = 0xFFFFFFFFFFFFFFFFULL;
	// First do a rough check to see if we think that any branch isl. are needed.
if ( _s_log ) fprintf(stderr, "ld: checking for poss branch isl.\n");
	if (! mightNeedBranchIslands(opts, state))
		return;

	// If we've seen branches between sections (or even across them), then we need
	// insert island regions globally to the __TEXT segment.  Otherwise, it's
	// enough to make island regions local to the section that requires them.
	if ( seenCrossSectBr || (opts.outputKind() == Options::kPreload) ) {
		buildAddressMap(opts, state);
	}

	// scan sections for number of stubs
	unsigned stubCount = 0;
	for (std::vector<ld::Internal::FinalSection*>::iterator sit=state.sections.begin(); sit != state.sections.end(); ++sit) {
		ld::Internal::FinalSection* sect = *sit;
		if ( sect->type() == ld::Section::typeStub )
			stubCount = sect->atoms.size();
	}

	// Build a list of regions into which branch islands can be inserted as required.
	findIslandInsertionPoints(opts, state);

	// scan sections and add island to each code section
	islandCount = 0;
	for (std::vector<ld::Internal::FinalSection*>::iterator sit=state.sections.begin();
		 sit != state.sections.end(); ++sit) {
		ld::Internal::FinalSection* sect = *sit;
		if ( sect->type() == ld::Section::typeCode ) 
			makeIslandsForSection(opts, state, sect, stubCount);
	}

	int regionIndex = 0;
    if ( islandCount == 0 ) {
		if ( _s_log ) fprintf(stderr, "ld: a bit surprising that we didn't need any branch islands after all\n");
		return;
    }

	// insert islands into their sections and adjust section offsets
	if ( _s_log ) fprintf(stderr, "ld: %u branch islands required in %u regions\n",
							islandCount, kIslandRegionsCount);

	while (regionIndex < kIslandRegionsCount) {
		// Accumulate the atoms to be added to a section.
		ld::Internal::FinalSection* sect = branchIslandInsertionSections[regionIndex];
		std::vector<const ld::Atom*> newAtomList;
		newAtomList.clear();
		newAtomList.reserve(sect->atoms.size()+islandCount);

		for (std::vector<const ld::Atom*>::iterator ait=sect->atoms.begin();
			 ait != sect->atoms.end(); ait++) {
			const ld::Atom* atom = *ait;
			newAtomList.push_back(atom);
			if ( (regionIndex < kIslandRegionsCount)
				 && (atom == branchIslandInsertionPoints[regionIndex]) ) {
				assert(branchIslandInsertionSections[regionIndex] == sect && "wrong section seen?");
				std::vector<const ld::Atom*>* islands = regionsIslands[regionIndex];
	if ( _s_log ) fprintf(stderr, "ld: inserted %d islands after %s (0x%08llx)\n",
							islands->size(), atom->name(), sAtomToAddress[atom]+atom->size());
				newAtomList.insert(newAtomList.end(), islands->begin(), islands->end());
				++regionIndex;
			}
		}
		// swap in new list of atoms for __text section
		if (!newAtomList.empty()) {
			sect->atoms.clear();
			sect->atoms = newAtomList;
		}
	}
}


} // namespace branch_island
} // namespace passes 
} // namespace ld 
