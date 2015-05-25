/*
 * opencog/atoms/SatisfactionLink.h
 *
 * Copyright (C) 2015 Linas Vepstas
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_SATISFACTION_LINK_H
#define _OPENCOG_SATISFACTION_LINK_H

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atomspace/Link.h>
#include <opencog/atoms/bind/LambdaLink.h>
#include <opencog/query/PatternMatchCallback.h>

namespace opencog
{
/** \addtogroup grp_atomspace
 *  @{
 *
 * Experimental SatisfactionLink class. This is a rough sketch for how things
 * like this might be done. It is not necessarily a good idea, and might
 * be replaced by something completely different, someday ...
 */

/// The SatisfactionLink is used to specify a list of variables, and a
/// pattern (containing those variables) that is to be grounded
/// (satisfied).  Thus, it resembles a LambdaLink, with the difference
/// being that it has a very specific semantics: the pattern is to be
/// grounded!
///
/// It is similar to a BindLink, except that a BindLink also causes an
/// implication to be performed: after a grounding is found, the
/// BindLink then causes the implication to run with the resultant
/// grounding.  The SatisfactionLink does not specify what should happen
/// with the grounding, although the (cog-satisfy) scheme call returns
/// a truth value.
class SatisfactionLink : public LambdaLink
{
protected:
	/// The actual clauses. Set by validate_clauses()
	Handle _hclauses;
	std::vector<Handle> _clauses;

	/// The graph components. Set by validate_clauses()
	std::vector<Handle> _virtuals;
	std::vector<Handle> _nonvirts;
	std::set<std::vector<Handle>> _components;

	// Validate the clauses inside the body
	void unbundle_clauses(const Handle&);
	void validate_clauses(std::set<Handle>& vars,
	                      std::vector<Handle>& clauses);

	void check_connectivity(const std::set<std::vector<Handle>>&);

	SatisfactionLink(Type, const HandleSeq&,
	         TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	         AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	void init(void);

public:
	SatisfactionLink(const HandleSeq&,
	         TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	         AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	SatisfactionLink(const Handle& varcdecls, const Handle& body,
	         TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	         AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	SatisfactionLink(Link &l);

	void satisfy(PatternMatchCallback *);
};

typedef std::shared_ptr<SatisfactionLink> SatisfactionLinkPtr;
static inline SatisfactionLinkPtr SatisfactionLinkCast(const Handle& h)
	{ AtomPtr a(h); return std::dynamic_pointer_cast<SatisfactionLink>(a); }
static inline SatisfactionLinkPtr SatisfactionLinkCast(AtomPtr a)
	{ return std::dynamic_pointer_cast<SatisfactionLink>(a); }

// XXX temporary hack ...
#define createSatisfactionLink std::make_shared<SatisfactionLink>

/** @}*/
}

#endif // _OPENCOG_SATISFACTION_LINK_H
