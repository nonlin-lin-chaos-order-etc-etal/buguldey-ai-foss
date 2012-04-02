/*----------------------------------------------------------------------
Oberon2 compiler for x86
Copyright (c) 2012 Evgeniy Grigorievitch Philippov
Distributed under the terms of GNU General Public License, v.3 or later

Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2, or (at your option) any 
later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than 
Coco/R itself) does not fall under the GNU General Public License.
-----------------------------------------------------------------------*/

#if !defined(Oberon_COCO_PARSER_H)
#define Oberon_COCO_PARSER_H

namespace Oberon {class CodeGenerator;}
#include "SymbolTable.h"
#include "CodeGenerator.h"
#include "wchar.h"

#include "Scanner.h"

namespace Oberon {

class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
//public:
	//void addParserListener(const ParserListener *const parserListener){this->parserListener=parserListener;}
private:
	enum {
		_EOF=0,
		_ident=1,
		_badString=2,
		_string=3,
		_integer=4,
		_real=5,
		_character=6
	};
	int maxT;

	//const ParserListener *parserListener;
	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

int // operators
	  illegal_operator, plus, minus, times, slash, equals, less, greater, orOperation, notEquals, lessOrEqual, greaterOrEqual, in, is, divOp, modOp, ampersand;

	int // types
	  undef, integer, boolean;

	int // object kinds
	  var, proc;

	int // opcodes
	  ADD,  SUB,   MUL,   DIV,   EQU,  LSS, GTR, NEG,
	  LOAD, LOADG, STO,   STOG,  CONST,
	  CALL, RET,   ENTER, LEAVE,
	  JMP,  FJMP,  READ,  WRITE;
	  
	struct ExprParams{
		int type;
	};
	
	SymbolTable   *tab;
	CodeGenerator *gen;

	void Err(const wchar_t* msg) {
		errors->Error(la->line, la->col, msg);
	}

	void InitDeclarations() { // it must exist
		
		// operators
		illegal_operator = -1;
		plus = 0; minus = 1; times = 2; slash = 3; equals = 4; less = 5; greater = 6; orOperation = 7;
	  	notEquals = 8; lessOrEqual = 9; greaterOrEqual=10; in=11; is=12;
	  	divOp=13; modOp=14; ampersand=15;
		 
		undef = 0; integer = 1; boolean = 2; // types
		var = 0; proc = 1; // object kinds

		// opcodes
		ADD  =  0; SUB   =  1; MUL   =  2; DIV   =  3; EQU   =  4; LSS = 5; GTR = 6; NEG = 7;
		LOAD =  8; LOADG =  9; STO   = 10; STOG  = 11; CONST = 12;
		CALL = 13; RET   = 14; ENTER = 15; LEAVE = 16;
		JMP  = 17; FJMP  = 18; READ  = 19; WRITE = 20;
	}


  
/*--------------------------------------------------------------------------*/


	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void number();
	void Relation(int &op);
	void AddOp(int &op);
	void MulOp(int &op);
	void Expr(ExprParams *expr);
	void SimpleExpr(ExprParams *expr);
	void ConstExpr(ExprParams *expr);
	void ImportList();
	void DeclSeq();
	void ConstDecl();
	void TypeDecl();
	void VarDecl();
	void ProcDecl();
	void ForwardDecl();
	void IdentDef();
	void Type();
	void IdentList();
	void Receiver();
	void FormalPars();
	void StatementSeq();
	void FPSection();
	void Qualident();
	void FieldList();
	void Statement();
	void Case();
	void CaseLabels();
	void Guard();
	void Term();
	void Factor();
	void DesignatorMaybeWithExprList();
	void Set();
	void ExprList();
	void QualidentOrOptionalExprList();
	void Element();
	void Module();
	void Ident(wchar_t* &name);
	void Oberon();

	void Parse();

}; // end Parser

} // namespace


#endif

