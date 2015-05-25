/*
 * opencog/atoms/LambdaLink.h
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

#ifndef _OPENCOG_LAMBDA_LINK_H
#define _OPENCOG_LAMBDA_LINK_H

#include <map>

#include <opencog/atoms/bind/VariableList.h>

namespace opencog
{
/** \addtogroup grp_atomspace
 *  @{
 *
 * Experimental LambdaLink class. This is a rough sketch for how things
 * like this might be done. It is not necessarily a good idea, and might
 * be replaced by something completely different, someday ...
 */

/// The LambdaLink consitsts of two parts: A variable declaration,
/// wgich must conform to current variable declaration standards: i.e.
/// it must be either a single VariableNode, a single TypedVariableLink,
/// or a VariableLink.  This is then followed by a body, of any
/// arbitrary form.  This class does little other than to check for
/// the above-described format; it will throw an error if an ill-formed
/// LambdaLink is inserted into the atomspace.  In addition to the
/// above, it also unpacks the variable declarations, using the
/// VariableList class as a helper class to do that unpacking.
/// As usual, the unpacked variables act as a memo or cache, speeding
/// up later calculations.
class LambdaLink : public VariableList
{
protected:
	/// Handle of the topmost variable declaration.
	VariableListPtr _vardecl;

	/// Handle of the body of the expression.
	Handle _body;

	// Extract variable decls and the body.
	void unbundle_body(const Handle&);

	LambdaLink(Type, const HandleSeq&,
	           TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	           AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	void init(const HandleSeq&);
public:
	LambdaLink(const HandleSeq&,
	           TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	           AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	LambdaLink(const Handle& varcdecls, const Handle& body,
	           TruthValuePtr tv = TruthValue::DEFAULT_TV(),
	           AttentionValuePtr av = AttentionValue::DEFAULT_AV());

	LambdaLink(Link &l);

	Handle substitute (const HandleSeq& seq)
	{
		return _vardecl->substitute(_body, seq);
	}
};

typedef std::shared_ptr<LambdaLink> LambdaLinkPtr;
static inline LambdaLinkPtr LambdaLinkCast(const Handle& h)
	{ AtomPtr a(h); return std::dynamic_pointer_cast<LambdaLink>(a); }
static inline LambdaLinkPtr LambdaLinkCast(AtomPtr a)
	{ return std::dynamic_pointer_cast<LambdaLink>(a); }

// XXX temporary hack ...
#define createLambdaLink std::make_shared<LambdaLink>

/** @}*/
}

#endif // _OPENCOG_LAMBDA_LINK_H
