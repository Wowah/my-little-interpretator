#include <iostream>
#include <string.h>
#include <stack>
#include <iterator>
#include <stdio.h>

using namespace std;

enum type_of_lex {	//типы лексем
	LEX_NULL, /*0*/
	LEX_END, /*1*/
	LEX_BEGIN, /*2*/
	LEX_PROGRAM, /*3*/
	LEX_FUNCTION, /*4*/
	LEX_VAR, /*5*/
	LEX_INT, /*6*/
	LEX_BOOL, /*7*/
	LEX_IF, /*8*/
	LEX_THEN, /*9*/
	LEX_ELSE, /*10*/
	LEX_WHILE, /*11*/
	LEX_DO, /*12*/
	LEX_TRUE, /*13*/
	LEX_FALSE, /*14*/
	LEX_READ, /*15*/
	LEX_WRITE, /*16*/
	LEX_FIN, /*17*/
	LEX_SEMICOLON, /*18*/
	LEX_COMMA, /*19*/
	LEX_PLUS, /*20*/
	LEX_MINUS, /*21*/
	LEX_TIMES, /*22*/
	LEX_SLASH, /*23*/
	LEX_OR,/*24*/
	LEX_AND, /*25*/
	LEX_NOT, /*26*/
	LEX_COLON, /*27*/
	LEX_LSS, /*28*/
	LEX_GTR, /*29*/
	LEX_LEQ, /*30*/
	LEX_NEQ, /*31*/
	LEX_EQ, /*32*/
	LEX_ASSIGN, /*33*/
	LEX_GEQ, /*34*/
	LEX_OPEN_ROUND_BRACKETS, /*35*/ 
	LEX_CLOSE_ROUND_BRACKETS, /*36*/
	LEX_NUM, /*37*/
	LEX_ID, /*38*/
	POLIZ_LABEL, /*39*/
	POLIZ_ADDRESS, /*40*/
	POLIZ_GO, /*41*/
	POLIZ_FGO /*42*/
};

int level; //область видимости

void
print(stack<type_of_lex> st)
{
	type_of_lex x;
	if (!st.empty()) {
		x = st.top();
		st.pop();
		cout << x << endl;
		print(st);
		st.push(x);
	} else {
		cout << "------------------------------------\n";
	}
}
	
class Function {	//класс функций и их параметров (аргументы, адрес начала/конца)
	char *name;
	int number_of_argument;
	int arguments[100];
	int top;
	int begin;
	int end;
public:
	Function(const char *nm = "main", int numb = 1) {
		top = 0;
		name = new char[strlen(nm) + 1];
		for (uint i = 0; i < strlen(nm); i++)
		{
			name[i] = nm[i];
		}
		name[strlen(nm)] = '\0';
		number_of_argument = numb;
	}
	
	Function(const Function &a) {
		name = new char[strlen(a.name) + 1];
		for (uint i = 0; i < strlen(a.name); i++)
		{
			name[i] = a.name[i];
		}
		name[strlen(a.name)] = '\0';
		number_of_argument = a.number_of_argument;
		top = a.top;
		begin = a.begin;
		end = a.end;
	}
	
	~Function() {
		delete []name;
	}
	
	int get_numb() { return number_of_argument; }
	
	void set_numb(int n) { 
		number_of_argument = n; 
	}
	
	const char * get_name() {
		char *ans = new char[strlen(name) + 1];
		for (uint i = 0; i < strlen(name); i++)
		{
			ans[i] = name[i];
		}
		ans[strlen(name)] = '\0';
		return ans;
	}
		 
	void set_name(const char *nm) {
		delete []name;
		name = new char[strlen(nm) + 1];
		for (uint i = 0; i < strlen(nm); i++)
		{
			name[i] = nm[i];
		}
		name[strlen(nm)] = '\0';
	}
	
	void put_arg(int x) {
		cout << "Я попал в put_arg\n";
		arguments[top++] = x;
	}
	
	int operator[](int i) { return arguments[i]; }
	
	void set_begin(int start) { begin = start; }
	
	int get_begin() { return begin; }
	
	void set_end(int fin) { end = fin; }
	
	int get_end() { return end; }
};
	
class tabl_func {		//класс таблицы функций
	Function *arr;
	int size;
	int top;
public:
	tabl_func ( int max_size ) {
		arr = new Function [ size = max_size ];
		top = 1;
	}
	~tabl_func ( ) { delete [ ] arr; }
	Function & operator [ ] ( int k ) { return arr [ k ]; }
	int put (const char * buf);
	int find (const char *name);
};

int tabl_func::put(const char * buf) {
	for (int j = 1; j < top; j++) {
		if (!strcmp( buf,arr[j].get_name())) return j;
	}
	arr [ top ].set_name ( buf );
	top++;
	return top - 1;
}

int tabl_func::find(const char *name) {
	for (int j = 1; j < top; j++) {
		if (!strcmp( name, arr[j].get_name())) return j;
		cout << "Я ищу функцию" << name << " ~ " << arr[j].get_name() << endl;
	}
	return 0;
}

tabl_func TF(100);
		
class Lex {		//основной класс: лексемы
	type_of_lex t_lex;
	int v_lex;
public:
	Lex ( type_of_lex t = LEX_NULL, int v = 0) {
		t_lex = t;
		v_lex = v;
	}
	
	type_of_lex
	get_type ( ) 
	{
		return t_lex;
	}
	
	int 
	get_value ( ) 
	{
		return v_lex;
	}
	
	friend ostream& 
	operator << (ostream & s, Lex l ) 
	{
		s << '(' << l.t_lex << ',' << l.v_lex << ");" ;
		return s;
	}
};

class Ident {		//класс идентификаторов
	char *name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
	int place;
public:
	Ident ( ) { declare = false; assign = false; name = new char[1];}
	
	~Ident ( ) { delete []name; }
	
	char *get_name ( ) { return name; }
	
	void put_name ( const char * n ) 
	{
		delete []name;
		name = new char [ strlen ( n ) + 1];
		strcpy ( name, n );
	}
	bool get_declare ( ) { return declare; }
	
	void put_declare ( ) { declare = true; }
	
	type_of_lex get_type ( ) { return type; }
	
	void put_type ( type_of_lex t ) { type = t; }
	
	bool get_assign ( ) { return assign; }
	
	void put_assign ( ) { assign = true; }
	
	int get_value ( ) { return value; }
	
	void put_value ( int v ) { value = v; }
	
	int get_place () { return place; }
	
	void put_place (int level) { place = level; }
};

class tabl_ident{		//класс таблицы идентификаторов
	Ident *p;
	int size;
	int top;
	bool new_decl;
public:
	tabl_ident ( int max_size ) {
		p = new Ident [ size = max_size ];
		top = 1;
		new_decl = true;
	}
	~tabl_ident ( ) { delete [ ] p; }
	Ident & operator [ ] ( int k ) { return p [ k ]; }
	int put (const char * buf) {
		for (int j = 1; j < top; j++) {
			if (!strcmp( buf,p[j].get_name()) && level == p[j].get_place()) return j;
		}
		if (!new_decl) {
			for (int j = 1; j < top; j++)
			{
				if (!strcmp( buf,p[j].get_name()) && p[j].get_place() == 0) return j;
			}
		}
		p [ top ].put_name ( buf );
		p [ top ].put_place (level);
		top++;
		return top - 1;
	}
	int get_top() { return top; }
	void new_decl_on() { new_decl = true; }
	void new_decl_off() { new_decl = false; }
};

class Scanner {			//класс сканер: создан для извлечения из файла лексемы
	enum state { H, IDENT, NUMB, COM, ALE, DELIM, NEQ };
	static char* TW [ ];
	static type_of_lex words [];
	static char * TD [ ];
	static type_of_lex dlms [];
	state CS;
	FILE *fp;
	char c;
	char buf [ 80 ];
	int buf_top;
	int count;
	bool raw;
	void clear ( ) ;
	void add ( );
	int look (const char * buf, char ** list);
	void gc ( ) { c = fgetc ( fp ); }
public:
	Scanner (const char * program);
	Lex get_lex ();
};

void
Scanner::clear ( ) 
{
	buf_top = 0;
	for (int j = 0; j < 80; j++ )
		buf [ j ] = '\0';
}

void
Scanner::add ( ) 
{
	buf [ buf_top ++ ] = c;
}

int
Scanner::look (const char * buf, char * * list) 
{
	int i = 0;
	while (list [ i ]) {
		if ( ! strcmp (buf, list [ i ] ) )
			return i;
		i++;
		//if (list[i]) {
			//cout << buf << ' ' << list[i] << endl;
		//}
	}
	return 0;
}

Scanner::Scanner (const char * program) {
	fp = fopen ( program, "r" );
	CS = H;
	raw = false;
	count = 0;
	clear();
}

char * Scanner:: TW [ ] =		//служебные слова
{ "wowah", "false\0", "true\0", "and\0","begin\0","bool\0","do\0",
	
"else\0","end\0", "if\0","int\0","not\0","or\0","program\0","read\0",

"then\0","var\0","while\0","write\0", "function\0", NULL};

type_of_lex Scanner::words[ ] =		//и их тип лексемы
{ LEX_NULL, LEX_FALSE, LEX_TRUE, LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, 
	
LEX_ELSE, LEX_END, LEX_IF, LEX_INT, LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ,

LEX_THEN, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_FUNCTION };

char * Scanner:: TD [ ] = { "wowah", ";\0", ".\0", ",\0", ":\0", ":=\0", "(\0",

 ")\0", "=\0", "<\0", ">\0", "+\0", "-\0", 		//разделители
 
 "*\0", "/\0", "<=\0", ">=\0", NULL};

type_of_lex Scanner::dlms[ ] =		//и их тип лексемы
{ LEX_NULL, LEX_SEMICOLON, LEX_FIN, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_OPEN_ROUND_BRACKETS,
	
LEX_CLOSE_ROUND_BRACKETS, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS,
	
LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_GEQ };

tabl_ident TID (100);		//таблица идентификаторов

Lex Scanner::get_lex ( ) {		//метод извлечения лексемы из файлы
	int d, j;
	CS = H;
	do {
		if (!raw) {
			gc();
			count++;
		}
		switch(CS) {
			case H:
				raw = false;
				if(c==' '||c=='\n'||c=='\r'||c =='\t') {}
				else if (isalpha(c)) {
					clear(); 
					add(); 
					CS = IDENT;
				}
				else if (isdigit(c)) {d = c - '0'; CS = NUMB;}
				else if ( c == '{' ) { CS = COM; }
				else if ( c == ':' || c == '<' || c == '>') {clear(); add(); CS = ALE; }
				else if ( c == '.') return Lex (LEX_FIN);
				else if(c=='!'){clear(); add (); CS = NEQ;}
				else {
					CS = DELIM;
					raw = true;
				}
				break;
			case IDENT:
				if ( isalpha(c) || isdigit(c) ) {
					add();
				} else if ( (j = look (buf, TW)) ) {
					raw = true;
					//cout << "Я идентификатор и мой номер:" << j << endl;
					return Lex (words[ j ], j-1);
				}
				else {
					j = TID.put (buf);
					raw = true;
					return Lex (LEX_ID, j);
				}
				break;
			case NUMB:
				if ( isdigit (c) ) {
					d = d * 10 + (c - '0'); }
				else {
					raw = true;
					return Lex ( LEX_NUM, d);
				}
				break;
			case COM:
				if ( c == '}' ) { CS = H; }
				else
				if (c == '.' || c == '{' ) throw c;
				break;
			case ALE:
				if ( c == '=' ) {
					add();
					j = look ( buf, TD );
					return Lex ( dlms [ j ], j);
				} else {
					raw = true;
					j = look (buf, TD);
					return Lex ( dlms [ j ], j );
				}
				break;
			case NEQ:
				if ( c == '=' ) {
					add(); 
					j = look ( buf, TD );
					return Lex ( LEX_NEQ, j );
				} else throw '!';
				break;
			case DELIM:
				clear( );
				add( );
				if ( (j = look(buf, TD)) ) {
					raw = false;
					return Lex ( dlms [ j ], j );
				}
				else {
					throw c;
				}
				break;
		} //end switch
		//raw = false;
		//cout << "YA TUT\n";
		//cout << count << ':' << c << endl;
	} while ( true );
}

class Poliz{		//класс ПОЛИЗ: хранит последовательность лексем для их последующего исполнения	
	Lex *p;			
	int size;
	int free;
public:
	Poliz(int max_size){p = new Lex[size = max_size]; free = 0;}
	~Poliz ( ) { delete [ ] p; }
	void put_lex ( Lex l ) { p [ free ] = l; free++; }
	void put_lex ( Lex l, int place ) { p [ place ] = l; }
	void blank ( ) { free++; }
	int get_free ( ) { return free; }
	Lex& operator [ ] ( int index ) {
		if ( index > size ) throw "POLIZ:out of array";
		else if ( index > free) throw "POLIZ:indefinite element of array";
		else return p [ index ];
	}
	void print ( ) {
		for ( int i = 0; i < free; i++)
			cout << p [ i ];
	}
};

class Parser {		//класс парсер: переводит исходную последовательность
					//в последовательность лексем методом рекурсивного спуска
	Lex curr_lex;
	type_of_lex c_type;
	int c_val;
	int count;
	bool is_argument;
	Scanner scan;
	stack <int> st_int;
	stack <type_of_lex> st_lex;
	void P(); void D1(); void D (); void B (); void B1(); void S ();
	void E(); void E1(); void T(); void F();
	void dec ( type_of_lex type); void check_id ();
	void check_op (); void check_not (); void eq_type ();
	void eq_bool (); void check_id_in_read ();
	void gl ( ) {
		curr_lex = scan.get_lex ();
		c_type = curr_lex.get_type ();
		c_val = curr_lex.get_value ();
		cout << curr_lex << endl;
	}
public:
	Poliz prog;
	Parser (const char *program) : scan (program), prog (1000) { }
	void analyze ( );
};

void Parser::analyze () {
	level = 0;
	is_argument = false;
	gl ();
	P ();
	prog.print ();
	cout << endl << "Yes!!!" << endl;
}

void Parser::dec ( type_of_lex type ) {		//метод объявления новых переменных
	int i;
	while ( ! st_int.empty ( )) {
		i = st_int.top ();
		st_int.pop();
		if ( TID [ i ].get_declare () && TID[ i ].get_place() == level ) throw "twice";
		else {
			if (is_argument) {
				TF[ level ].put_arg(i);
			}
			TID[ i ].put_declare ();
			TID[ i ].put_type (type);
		}
	}
}

void Parser::check_id ( ) {		//проверка идентификатора
	if (TID [ c_val ].get_declare ( ))
		st_lex.push (TID [ c_val ].get_type ( ) );
	else throw "not declared";
}

void Parser::check_op ( ) {		//проверка операции, совпадение типов операндов и операции
	print(st_lex);
	type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
	t2 = st_lex.top( );
	st_lex.pop();
	op = st_lex.top( );
	st_lex.pop();
	t1 = st_lex.top( );
	st_lex.pop();
	if (op == LEX_PLUS || op == LEX_MINUS
		||op == LEX_TIMES ||op == LEX_SLASH || 
		op == LEX_LSS || op == LEX_GTR || op == LEX_GEQ || op == LEX_LEQ) {
		t = LEX_INT;
		if (op == LEX_PLUS || op == LEX_MINUS
		||op == LEX_TIMES ||op == LEX_SLASH) {
			r = LEX_INT;
		} else {
			r = LEX_BOOL;
		}
	}	
	if (op == LEX_OR || op == LEX_AND)
		t = r = LEX_BOOL;
	if (t1 == t2) {
		if (t1 == t) {
			st_lex.push( r );
		} else if (op != LEX_EQ && op != LEX_NEQ) {
			cout << t1 << ' ' << op << ' ' << t2 << ' '  << t << endl;
			throw "wrong types are in operation";
		} else {
			st_lex.push(t1);
		}
	}
	prog.put_lex (Lex (op) );
}

void Parser::check_not ( ) {
	type_of_lex i = st_lex.top();
	st_lex.top();
	if (i != LEX_BOOL)
		throw "wrong type is in not";
	else {
		st_lex.push (LEX_BOOL);
		prog.put_lex (Lex (LEX_NOT));
	}
}

void Parser::eq_type () {	//проверка типов операндов при присваивании
	type_of_lex i = st_lex.top(), j;
	st_lex.pop();
	j = st_lex.top();
	st_lex.pop();
	if ( i != j )
		throw "wrong types are in :=";
}

void Parser::eq_bool () {	//проверка на логическое выражение
	type_of_lex i = st_lex.top();
	st_lex.pop();
	if ( i != LEX_BOOL )
		throw "expression is not boolean";
}

void Parser::check_id_in_read () {
	if ( !TID [c_val].get_declare( ) ) 
		throw "not declared";
}

void Parser::P () {
	if (c_type == LEX_PROGRAM)
		gl ();
	else {
		cout << "program\n";
		throw curr_lex;
	}
	D1();
	if (c_type == LEX_SEMICOLON)
		gl ();
	else {
		cout << "';'\n";
		throw curr_lex;
	}
	B ();
	if (c_type != LEX_FIN)
		throw curr_lex;
}

void Parser::D1 () {
	if (c_type != LEX_VAR) {
		cout << "var\n";
		throw curr_lex;
	}
	count = 0;
	TID.new_decl_on();
	do {
		gl();
		D();
	} while (c_type == LEX_COMMA);
	if (level != 0 && is_argument) {
		TF [ level ].set_numb(count);
	}
	TID.new_decl_off();
}

void Parser::B () {
	int j, fun_pos, pl1;
	if (c_type == LEX_FUNCTION) {
		pl1 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		gl();
		if (c_type != LEX_ID) {
			throw curr_lex;
		}
		if (TID[ c_val ].get_declare()) {
			throw "twice";
		} else {
			TID [ c_val ].put_declare();
		}
		j = TF.put(TID[c_val].get_name());
		level = j;
		fun_pos = c_val;
		gl();
		if (c_type != LEX_OPEN_ROUND_BRACKETS) {
			throw "Expected '('";
		}
		is_argument = true;
		gl();
		D1();
		is_argument = false;
		if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
			throw "Expected ')'";
		}
		gl();
		if (c_type != LEX_COLON) {
			throw curr_lex;
		}
		gl();
		if (c_type == LEX_INT) {
			TID [fun_pos].put_type(LEX_INT);
		} else if (c_type == LEX_BOOL) {
			TID [fun_pos].put_type(LEX_BOOL);
		} else {
			throw curr_lex;
		}
		gl();
		D1();
		if (c_type != LEX_SEMICOLON) {
			throw curr_lex;
		}
		gl();
		TF[ j ].set_begin(prog.get_free());
		B1();
		TF[ j ].set_end(prog.get_free());
		prog.blank();
		prog.put_lex(Lex(POLIZ_GO));
		while (c_type == LEX_SEMICOLON) {
			level = 0;
			gl();
			if (c_type != LEX_FUNCTION) {
				throw curr_lex;
			}
			gl();
			if (c_type != LEX_ID) {
				throw curr_lex;
			}
			if (TID[ c_val ].get_declare()) {
				throw "twice";
			} else {
				TID [ c_val ].put_declare();
			}
			j = TF.put(TID[c_val].get_name());
			level = j;
			fun_pos = c_val;
			gl();
			if (c_type != LEX_OPEN_ROUND_BRACKETS) {
				throw "Expected '('";
			}
			is_argument = true;
			gl();
			D1();
			is_argument = false;
			if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
				throw "Expected ')'";
			}
			gl();
			if (c_type != LEX_COLON) {
				throw curr_lex;
			}
			gl();
			if (c_type == LEX_INT) {
				TID [fun_pos].put_type(LEX_INT);
			} else if (c_type == LEX_BOOL) {
				TID [fun_pos].put_type(LEX_BOOL);
			} else {
				throw curr_lex;
			}
			gl();
			D1();
			if (c_type != LEX_SEMICOLON) {
				throw curr_lex;
			}
			gl();
			TF[ j ].set_begin(prog.get_free());
			B1();
			TF[ j ].set_end(prog.get_free());
			prog.blank();
			prog.put_lex(Lex(POLIZ_GO));
		}
		level = 0;
		prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl1);
		B1();
	} else {
		B1();
	}
}
	
void Parser::B1() {
	if (c_type != LEX_BEGIN) {
		cout << "begin1\n";
		throw curr_lex;
	}
	do {
		gl();
		S ();
	} while (c_type == LEX_SEMICOLON);
	if (c_type != LEX_END) {
		cout << "end\n";
		throw curr_lex;
	}
	gl();
}	

void Parser::S () {
	int pl0, pl1, pl2, pl3;
	switch (c_type) {
		case LEX_ID:
			check_id();
			prog.put_lex(Lex(POLIZ_ADDRESS,c_val));
			gl();
			if (c_type != LEX_ASSIGN) {
				throw "Unknown command";
			}
			gl();
			E ();
			eq_type();
			prog.put_lex(Lex(LEX_ASSIGN));
			break;
		case LEX_READ:
			cout << "Я попал в read\n";
			gl();
			if (c_type != LEX_OPEN_ROUND_BRACKETS) {
				throw "In function 'read': expected '('";
			}
			gl();
			if (c_type != LEX_ID) {			
				throw "In function 'read': expected identificator";
			}
			check_id_in_read();
			prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
			gl();
			if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
				throw "In function 'read': expected ')'";
			}
			prog.put_lex(Lex(LEX_READ));
			gl();
			break;
		case LEX_WRITE:
			gl();
			if (c_type != LEX_OPEN_ROUND_BRACKETS) {
				throw "In function 'write': expected ')";
			}
			gl();
			E();
			if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
				throw "In function 'read': expected ')'";
			}
			prog.put_lex(Lex(LEX_WRITE));
			gl();
			break;
		case LEX_IF:
			gl();
			E();
			eq_bool();
			pl2 = prog.get_free();
			prog.blank();
			prog.put_lex(Lex(POLIZ_FGO));
			if (c_type != LEX_THEN) {
				throw "expected 'then'";
			}
			gl();
			S();
			pl3 = prog.get_free();
			prog.blank();
			prog.put_lex(Lex(POLIZ_GO));
			prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl2);
			if (c_type != LEX_ELSE) {
				throw "expected 'else'";
			}
			gl();
			S();
			prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl3);
			break;
		case LEX_WHILE:
			pl0 = prog.get_free();
			gl();
			E();
			eq_bool();
			pl1 = prog.get_free();
			prog.blank();
			prog.put_lex(Lex(POLIZ_FGO));
			if (c_type != LEX_DO) {
				throw "expected 'do'";
			}
			gl();
			S();
			prog.put_lex(Lex(POLIZ_LABEL, pl0));
			prog.put_lex(Lex(POLIZ_GO));
			prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl1);
			break;
		case LEX_BEGIN:
			B();
			break;
		default:
			cout << "YA TUT\n";
			throw "unknown command";
	}
}

void Parser::E() {
	E1();
	if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
		c_type == LEX_GEQ || c_type == LEX_LEQ || c_type == LEX_NEQ) {
		st_lex.push(c_type);
		gl();
		E1();
		check_op();
	}
}

void Parser::E1() {
	bool raw = false;
	do {
		if (raw) {
			gl();
		}
		T();
		if (raw) {
			check_op();
		}
		if (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
			st_lex.push(c_type);
			raw = true;
		}
	} while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR);
}

void Parser::T() {
	bool raw = false;
	do {
		if (raw) {
			gl();
		}
		F();
		if (raw) {
			check_op();
		}
		if (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
			st_lex.push(c_type);
			raw = true;
		}
	} while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND);
}
			
void Parser::F() {
	int j;
	Lex x;
	switch (c_type) {
		case LEX_ID:
			if (TF.find(TID [c_val] .get_name()) != 0) {
				j = TF.put(TID [c_val] .get_name());
				cout << "Я вызвал функцию с кол-вом аргументов " << TF[j].get_numb() << endl;
				cout << "И я нахожусь на уровне " << level << endl;
				x = curr_lex;
				gl();
				if (c_type != LEX_OPEN_ROUND_BRACKETS) {
					throw "Expected '('";
				}
				prog.put_lex(Lex(POLIZ_ADDRESS,TF[j][0]));
				gl();
				E();
				prog.put_lex(Lex(LEX_ASSIGN));
				for (int i = 1; i < TF[j].get_numb(); i++)
				{
					if (c_type != LEX_COMMA) {
						throw "Too low arguments in function";
					}
					prog.put_lex(Lex(POLIZ_ADDRESS,TF[j][i]));
					gl();
					E();
					prog.put_lex(Lex(LEX_ASSIGN));
				}
				if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
					throw "expected ')'";
				}
				gl();
				prog.put_lex(Lex(POLIZ_LABEL,TF[ j ].get_begin()));
				prog.put_lex(Lex(POLIZ_GO));
				prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),TF[ j ].get_end());
				prog.put_lex(x);
			} else {
				cout << "И я нахожусь на уровне " << level << endl;
				check_id();
				prog.put_lex(curr_lex);
				gl();
			}
			break;
		case LEX_NUM:
			st_lex.push(LEX_INT);
			prog.put_lex(curr_lex);
			gl();
			break;
		case LEX_TRUE: case LEX_FALSE:
			st_lex.push(LEX_BOOL);
			prog.put_lex(curr_lex);
			gl();
			break;
		case LEX_NOT:
			gl();
			F();
			check_not();
			break;
		case LEX_OPEN_ROUND_BRACKETS:
			gl();
			E();
			if (c_type != LEX_CLOSE_ROUND_BRACKETS) {
				throw "expected ')'";
			}
			gl();
			break;
		default:
			throw "Error!";
	}
}

void Parser::D ( ) {
	if (c_type != LEX_ID) {
		cout << "ID1\n";
		throw curr_lex;
	}
	else {
		count++;
		st_int.push ( c_val );
		gl ( );
		while (c_type == LEX_COMMA) {
			gl ( );
			if (c_type != LEX_ID) {
				cout << "ID2\n";
				throw curr_lex;
			}
			else {
				count++;
				st_int.push ( c_val ); 
				gl ( );
			}
		}
		if (c_type != LEX_COLON) {
			cout << "':'\n";
			throw curr_lex;
		}
		else {
			gl ( );
			if (c_type == LEX_INT) { 
				dec ( LEX_INT ); 
				gl ( ); 
			}
			else
			if (c_type == LEX_BOOL) { 
				dec ( LEX_BOOL ); 
				gl ( ); 
			}
			else {
				cout << "her znaet\n";
				throw curr_lex;
			}
		}
	}
}

class Executer {	//класс-выполнитель: занимается выполнением программы
					//путем обработки последовательности лексем в ПОЛИЗе
	Lex pc_el;
public:
	void execute (Poliz & prog);
};

void Executer::execute ( Poliz& prog ) {
	stack <int> args;
	int i, j, index = 0, size = prog.get_free ( );
	while ( index < size ) {
		pc_el = prog [ index ];
		switch ( pc_el.get_type () ) {
		case LEX_TRUE: case LEX_FALSE: case LEX_NUM:
		case POLIZ_ADDRESS: case POLIZ_LABEL:
			args.push ( pc_el.get_value () );
			break;
		case LEX_ID:
			i = pc_el.get_value ( );
			if ( TID [ i ].get_assign ( ) ) {
				args.push ( TID[i].get_value () );
				break;
			} else {
				cout << TID[ i ].get_name() << ':'; 
				throw "POLIZ: indefinite identifier";
			}
		case LEX_NOT:
			i = args.top();
			args.pop();
			args.push( !i );
			break;
		case LEX_OR:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j || i );
			break;
		case LEX_AND:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j && i );
			break;
		case POLIZ_GO:
			index = args.top() - 1;
			args.pop();
			break;
		case POLIZ_FGO:
			i = args.top();
			args.pop();
			if ( !args.top() ) index = i-1;
			args.pop();
			break;
		case LEX_WRITE:
			cout << args.top () << endl;
			args.pop();
			break;
		case LEX_READ: { int k;
			i = args.top ( );
			args.pop();
			if ( TID [ i ].get_type () == LEX_INT ) {
				cout << "Input int value for ";
				cout << TID[i].get_name () << endl;
				cin >> k;
			}
			else { 
				char j [ 20 ];
		rep:
				cout << "Input boolean value";
				cout << "(true or false) for";
				cout << TID [ i ].get_name ( ) << endl;
				cin >> j;
				if (!strcmp (j, "true")) k = 1;
				else if (!strcmp (j, "false")) k = 0;
				else {
					cout << "Error in input:true/false";
					cout << endl;
					goto rep; 
				}
			}
			TID [ i ].put_value (k);
			TID [ i ].put_assign ();
			break; 
		}
		case LEX_PLUS:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j + i );
			break;
		case LEX_TIMES:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j * i );
			break;
		case LEX_MINUS:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j - i );
			break;
		case LEX_SLASH:
			i = args.top ( );
			args.pop();
			if ( i ) { 
				j = args.top();
				args.pop();
				args.push(args.top ( ) / i);
				break;
			}
			else
			throw "POLIZ:divide by zero";
		case LEX_EQ:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j == i );
			break;
		case LEX_LSS:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j < i );
			break;
		case LEX_GTR:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j > i );
			break;
		case LEX_LEQ:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j <= i );
			break;
		case LEX_GEQ:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( j >= i );
			break;
		case LEX_NEQ:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			args.push ( i != j );
			break;
		case LEX_ASSIGN:
			i = args.top();
			args.pop();
			j = args.top();
			args.pop();
			TID[j].put_value(i);
			TID[j].put_assign(); 
			break;
		default: throw "POLIZ: unexpected elem";
		} //end of switch
		index++;
	}; //end of while
	cout << "Finish of executing!!!" << endl;
	//for (int i = 0; i < TID.get_top(); i++)
	//{
		//cout << TID[i].get_name() << ' ' << TID[i].get_place() << endl;
	//}
}

class Interpretator {
	Parser pars;
	Executer E;
public:
	Interpretator(char * program):pars(program){ };
	void interpretation ( );
};
	
void Interpretator :: interpretation ( ) {
	pars.analyze ( );
	E.execute ( pars.prog );
}

int
main(void)
{
	try {
		Interpretator I ("program.txt");
		I.interpretation ();
		return 0;
	}
	catch (char c) {
		cout << "unexpected symbol " << c << endl;
		return 1;
	}
	catch (Lex l) {
		cout << "unexpected lexeme"; cout << l << endl;
		if (l.get_type() == LEX_ID) {
			cout << TID[l.get_value()].get_name() << endl;
		}
		return 1;
	}
	catch (const char * source) {
		cout << source << endl;
		return 1;
	}
	return 0;
}
