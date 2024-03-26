// lexical analyzer
// to get token

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cctype>
using namespace std;

enum TokenKind {
	LPAREN =1,
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
	string text;
	int pos;
	int line;
	Token() {
		kind = OTHERS; text = ""; pos = 0; line = 0;
	}
	Token(TokenKind k, const string& s, int l=0, int p=0) {
		kind = k; text = s; pos = p; line = l;
	}
};

// TODO::move lexialAnalyzer.h
void initChType();
Token nextToken();
int nextCh();
TokenKind getKind(const string& s);

TokenKind ctyp[256];
Token token;
ifstream fin;

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



int main(int argc, char* argv[]) {
	if (argc == 1) exit(1);
	fin.open(argv[1]); if (!fin) exit(1);

	cout << "line    pos    kind  text\n";
	initChType();
	static int line = 0;
	static int pos = 0;
	for (token = nextToken(); token.kind != EofTkn; token = nextToken()) {
		cout << left << setw(7) << token.line
			<< left << setw(7) << token.pos
			<< left << setw(10) << token.kind
			<< " " << token.text << endl;
	}
	return 0;
}

void initChType() {
	int i;
	for (i = 0; i < 256; i++) ctyp[i] = TEXT; // temp
	for (i = '0'; i <= '9'; i++) ctyp[i] = TEXT;
	for (i = 'A'; i <= 'Z'; i++) ctyp[i] = TEXT;
	for (i = 'a'; i <= 'z'; i++) ctyp[i] = TEXT;
	ctyp[' '] = TEXT;
	ctyp['\t'] = TEXT;

	ctyp['('] = LPAREN;
	ctyp[')'] = RPAREN;
	ctyp['_'] = UNDERSCORE;
	ctyp['['] = LBRACKET;
	ctyp[']'] = RBRACKET;
	ctyp['`'] = BACKTICK;
	ctyp['*'] = ASTERIK;
	ctyp['>'] = GREATERTHAN;
	ctyp['<'] = LESSTHAN;
	ctyp['\n'] = NEWLINE;
	ctyp['-'] = DASH;
	ctyp['#'] = HASH;
	ctyp['!'] = BANG;

}



Token nextToken() {
	TokenKind kd;
	int num = 0;
	static int ch = nextCh();
	static int pos;
	static int line;
	int tmpPos;
	string txt = "";
	if (ch == EOF) return Token(EofTkn, txt, line, pos++);

	switch (ctyp[ch]) {
	case TEXT:
		for (; ctyp[ch] == TEXT; ch = nextCh())
			txt += ch;
		return Token(TEXT, txt, line, pos++);
		break;
	case NEWLINE:
		pos = 0;
		tmpPos = pos;
		txt += ch; ch = nextCh();
		return Token(NEWLINE, txt, line++, tmpPos);
		break;
	default:
		txt += ch; ch = nextCh();
		break;
	}
	kd = getKind(txt);
	if (kd == OTHERS) {
		cout << "잘못된 토큰: " << txt << endl; exit(1);
	}
		
	return Token(kd, txt, line, pos++);
}

int nextCh() {
	static int c = 0;
	if (c == EOF) return c;
	if ((c = fin.get()) == EOF) fin.close();
	return c;
}

TokenKind getKind(const string& s) {
	for (int i = 0; keyWordTable[i].keyKind != END_LIST; i++) {
		if (s == keyWordTable[i].keyName) return keyWordTable[i].keyKind;
	}
	return OTHERS;
}
