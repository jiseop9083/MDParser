// to get token

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
#include "tokenizer.h"
//using namespace std;

std::ifstream fin;

int main(int argc, char* argv[]) {
	if (argc == 1) exit(1);
	Tokenizer tokenizer;
	
	static int line = 0;
	static int pos = 0;

	std::vector<Token*> tkns = tokenizer.tokenize(argv[1]);
	std::cout << "line    pos    kind  text\n";
	std::cout << "size " << tkns.size() << std::endl;
	for (Token* token : tkns) {
		std::cout << std::left << std::setw(7) << token->line
			<< std::left << std::setw(7) << token->pos
			<< std::left << std::setw(10) << token->kind
			<< " " << token->text << std::endl;
	}
	return 0;
}

std::vector<Token*> Tokenizer::tokenize(const char* url) {
	fin.open(url);
	if (!fin) exit(1);
	tokens.clear();
	for (token = nextToken(); token->kind != EofTkn; token = nextToken()) {
		tokens.push_back(token);
	}
	return tokens;
}

void Tokenizer::initChType() {
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

Token* Tokenizer::nextToken() {
	TokenKind kd;
	int num = 0;
	static int ch = nextCh();
	static int pos;
	static int line;
	int tmpPos;
	std::string txt = "";
	if (ch == EOF) return new Token(EofTkn, txt, line, pos++);

	switch (this->ctyp[ch]) {
	case TEXT:
		for (; this->ctyp[ch] == TEXT; ch = nextCh())
			txt += ch;
		return new Token(TEXT, txt, line, pos++);
		break;
	case NEWLINE:
		pos = 0;
		tmpPos = pos;
		txt += ch; ch = nextCh();
		return new Token(NEWLINE, txt, line++, tmpPos);
		break;
	default:
		txt += ch; ch = nextCh();
		break;
	}
	kd = getKind(txt);
	if (kd == OTHERS) {
		std::cout << "잘못된 토큰: " << txt << std::endl; exit(1);
	}
		
	return new Token(kd, txt, line, pos++);
}

int Tokenizer::nextCh() {
	static int c = 0;
	if (c == EOF) return c;
	if ((c = fin.get()) == EOF) fin.close();
	return c;
}

TokenKind Tokenizer::getKind(const std::string& s) {
	for (int i = 0; keyWordTable[i].keyKind != END_LIST; i++) {
		if (s == keyWordTable[i].keyName) return keyWordTable[i].keyKind;
	}
	return OTHERS;
}
