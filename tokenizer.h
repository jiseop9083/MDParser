#pragma once

enum TokenKind {
	LPAREN = 1,
	RPAREN,
	UNDERSCORE,
	LBRACKET,
	RBRACKET,
	BACKTICK,
	ASTERIK,
	GREATERTHAN,
	LESSTHAN,
	NEWLINE,
	DASH,
	TEXT,
	HASH,
	BANG,
	OTHERS,
	EofTkn,
	END_LIST
};

struct Token {
	TokenKind kind;
	std::string text;
	int pos;
	int line;
	Token() {
		kind = OTHERS; text = ""; pos = 0; line = 0;
	}
	Token(TokenKind k, const std::string& s, int l = 0, int p = 0) {
		kind = k; text = s; pos = p; line = l;
	}
};



struct KeyWord {
	const char* keyName;
	TokenKind keyKind;
};

KeyWord keyWordTable[] = {
	{"(", LPAREN},
	{")", RPAREN},
	{"_", UNDERSCORE},
	{"[", LBRACKET},
	{"]", RBRACKET},
	{"`", BACKTICK},
	{"*", ASTERIK},
	{">", GREATERTHAN},
	{"<", LESSTHAN},
	{"\n", NEWLINE},
	{"-", DASH},
	{"#", HASH},
	{"!", BANG},
	{"", END_LIST},
};

class Tokenizer {

public:
	void initChType();
	Token* nextToken();
	std::vector<Token*> tokenize(const char* url);
	int nextCh();
	TokenKind getKind(const std::string& s);

	Tokenizer() {
		initChType();
	}


private:
	TokenKind ctyp[256];
	Token* token;
	std::vector<Token*> tokens;
};