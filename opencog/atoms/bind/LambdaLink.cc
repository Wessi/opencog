/*
 * LambdaLink.cc
 *
 * Copyright (C) 2009, 2014, 2015 Linas Vepstas
 *
 * Author: Linas Vepstas <linasvepstas@gmail.com>  January 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the
 * exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/atomspace/ClassServer.h>
#include <opencog/atoms/TypeNode.h>

#include "LambdaLink.h"

using namespace opencog;

void LambdaLink::init(const HandleSeq& oset)
{
	// Must have variable decls and body
	if (2 != oset.size())
		throw InvalidParamException(TRACE_INFO,
			"Expecting variabe decls and body, got size %d", oset.size());

	validate_vardecl(oset[0]);
	_body = oset[1];     // Body

	const Handle& vars = oset[0];
	_vardecl = VariableListCast(vars);  // VariableNode declarations
	if (NULL == _vardecl)
	{
		// Note: this uses the "crazy constructor"!!
		// It builds VarLists from single VariableNode's
		_vardecl = createVariableList(vars);
	}
	// _vardecl cannot be null here, after the above...
}

LambdaLink::LambdaLink(const HandleSeq& oset,
                       TruthValuePtr tv, AttentionValuePtr av)
	: VariableList(LAMBDA_LINK, oset, tv, av)
{
	init(oset);
}

LambdaLink::LambdaLink(const Handle& vars, const Handle& body,
                       TruthValuePtr tv, AttentionValuePtr av)
	: VariableList(LAMBDA_LINK, HandleSeq({vars, body}), tv, av)
{
	init(getOutgoingSet());
}

LambdaLink::LambdaLink(Type t, const HandleSeq& oset,
                       TruthValuePtr tv, AttentionValuePtr av)
	: VariableList(t, oset, tv, av)
{
	init(oset);
}

LambdaLink::LambdaLink(Link &l)
	: VariableList(l)
{
	// Type must be as expected
	Type tscope = l.getType();
	if (not classserver().isA(tscope, LAMBDA_LINK))
	{
		const std::string& tname = classserver().getTypeName(tscope);
		throw InvalidParamException(TRACE_INFO,
			"Expecting a LambdaLink, got %s", tname.c_str());
	}

	init(l.getOutgoingSet());
}

/* ================================================================= */
/**
 * Unpack a LambdaLink into vardecls and body
 * Very similar to the constructors.
 */
void LambdaLink::unbundle_body(const Handle& hlambda)
{
	// Must be non-empty.
	LinkPtr lbl(LinkCast(hlambda));
	if (NULL == lbl)
		throw InvalidParamException(TRACE_INFO,
			"Expecting a LambdaLink");

	// Type must be as expected
	Type tscope = hlambda->getType();
	if (not classserver().isA(tscope, LAMBDA_LINK))
	{
		const std::string& tname = classserver().getTypeName(tscope);
		throw InvalidParamException(TRACE_INFO,
			"Expecting a LambdaLink, got %s", tname.c_str());
	}

	init(lbl->getOutgoingSet());
}

/* ===================== END OF FILE ===================== */
