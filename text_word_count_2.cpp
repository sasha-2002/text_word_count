#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>

using namespace std;

#define CHAR_MAP_SIZE 27



struct trie_node 
{
	unsigned short int count;
	struct trie_node* kinder[CHAR_MAP_SIZE];
};

int char_to_map_index(char source)
{
	char tmp = tolower(source);
	if (tmp >= 'a' && tmp <= 'z') 
	{
		return tmp - 'a';
	}
	else if (tmp == char(39)) 
	{
		return 26;
	}
	else 
	{
		exit(2);
		
	}

}

char char_map_index_to_char(int char_map_index)
{
	if (char_map_index != 26) 
	{
		return 'a' + char_map_index;
	}
	else if (char_map_index == 26) 
	{
		return char(39);// '
	}
	return ' ';
}

bool is_word_separator(char source)
{
	if (source == ' ') { return true; }// not real >> not space
	if (source == '.') { return true; }
	if (source == '/') { return true; }
	if (source == char(92)) { return true; }//  ' \ '  //
	if (source == '\n') { return true; }
	if (source == '\t') { return true; }
	return false;
}



string normal(string inp) 
{
	int len = inp.length();
	string out= "";
	for (int i = 0; i < len; i++) 
	{
		if ((inp[i] >= 'a' && inp[i] <= 'z') || (inp[i] >= 'A' && inp[i] <= 'Z')) 
		{
			out += inp[i];
		}
		else if (inp[i] == char(39) && i < len - 2 && out.length() > 0) 
		{
			out += inp[i];
		}
	}
	return out;
}
typedef struct trie_node* trie_t;

int countWords = 0;

/////////////////////////////////////////////////////////////////////////////////////

trie_t trie_create(void) 
{
	trie_t node = new trie_node;

	if (node == NULL) 
	{
		return NULL;
	}

	node->count = 0;

	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		node->kinder[i] = NULL;
	}

	return node;
}

void trie_free(trie_t tree) 
{ 
	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		if (tree->kinder[i] != NULL) 
		{
			trie_free(tree->kinder[i]);
		}
	}
	delete tree;
}

void length_word(trie_t start, unsigned short int d, string t) 
{
	unsigned short int m = d;
	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		string h = t;
		if (start->kinder[i] != NULL) 
		{
			h += char_map_index_to_char(i);
			if (start->kinder[i]->count > 0 && m == 1) 
			{
				cout << h << "     " << start->kinder[i]->count << endl;
			}
			else 
			{
				length_word(start->kinder[i], m - 1, h);
			}
		}
	}
}


void add(trie_t root, string n) 
{
	unsigned short int l = n.length();
	trie_t tmp = root;
	for (unsigned short int i = 0; i < l; i++) 
	{
		
		unsigned short int g = char_to_map_index(n[i]);
		

		if (tmp->kinder[g] == NULL) 
		{
			tmp->kinder[g] = trie_create();
			tmp = tmp->kinder[g];
		}
		else 
		{
			tmp = tmp->kinder[g];
		}
	}
	tmp->count++;

}
void printCount(trie_t start, string g) 
{
	for (unsigned short int i = 0; i < CHAR_MAP_SIZE; i++) 
	{
		string h = g;
		if (start->kinder[i] != NULL) 
		{
			h += char_map_index_to_char(i);
					
			if (start->kinder[i]->count > 0) 
			{
				cout << h << "   " << start->kinder[i]->count << endl;
				countWords += start->kinder[i]->count;
			}
			printCount(start->kinder[i], h);
		}
	}
}


string trie_get_first_word(trie_t root, string h = "")
{
	trie_t tmp = root;
	if (tmp == NULL) 
	{
		return " ";
	}
	for (int i = 0; i < CHAR_MAP_SIZE;i++) 
	{
		if (tmp->kinder[i] != NULL) 
		{
			h += char_map_index_to_char(i);
			if (tmp->kinder[i]->count > 0) 
			{
				return h;
			}
			return trie_get_first_word(tmp->kinder[i], h);
		}
	}
	return h;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string trie_get_next_word(int index_er, trie_t root, trie_t tmp, string current, string out = "", int count = 0) 
{
	int g;
	if (count < current.length()) 
	{
		g = char_to_map_index(current[count]);
	}
	else 
	{
		g = 0;
	}
	count++;
	while (g < CHAR_MAP_SIZE) 
	{
		if (tmp->kinder[g] != NULL && (count == current.length()+1 ? g > index_er:true)) 
		{
			out += char_map_index_to_char(g);
			if (tmp->kinder[g]->count > 0 && count > current.length()) 
			{
				return out;
			}
			return trie_get_next_word(index_er, root, tmp->kinder[g], current, out, count);
		}
		g++;
	}
	if (current=="") 
	{
		return "";
	}
	char m = current[current.length() - 1];
	current.pop_back();
	return trie_get_next_word(char_to_map_index(m), root, root, current);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short int find_words(trie_t root, string n)
{
	trie_t tmp = root;
	if (tmp == NULL) 
	{
		return 0;
	}
	unsigned short int len = n.length();
	for (unsigned short int i = 0; i < len; i++) 
	{
		if (tmp == NULL) 
		{
			return 0;
		}
		unsigned short int g = char_to_map_index(n[i]);
		tmp = tmp->kinder[g];
	}
	if (tmp->count > 0) 
	{
		return tmp->count;
	}
	else 
	{
		return 0;
	}

}

////////////////////////////////////////////////////////////////////////////////////////


int main() 
{
	trie_t root = NULL;
	root = trie_create();
	string input;
	cout << "input - (.txt) " << endl;
	cin >> input;
	time_t start_time = clock();
	ifstream file;
	file.open(input);
	if (!file.is_open()) 
	{
		cout << "eror" << endl;
	}

	char buff;
	string str;
	
	for (buff = file.get(); !file.eof();buff=file.get()) 
	{
		str = "";
		for (; !is_word_separator(buff) && !file.eof(); buff = file.get()) 
		{
			str += buff;
		}
		
		add(root, normal(str));
			
	}
	file.close();
	time_t end_time = clock();
	time_t search_time = end_time - start_time;
	cout << "TIME TRIE " << search_time << " ms" << endl;

	cout << "1 = find, 2 = PrintCount, 3 = length_word, 4 = new print words, 5 = printing by index  ";
	unsigned short int menu;
	
	cin >> menu;
	string inp;
	switch (menu) 
	{
	case 1:
		cout << "input please words   ";
		cin >> inp;
		cout << inp << "   " << find_words(root, inp) << endl;
		break;
	case 2:
		start_time = clock();
		printCount(root, "");
		end_time = clock();
		search_time = end_time - start_time;
		cout << "TIME PRINT  " << search_time << " ms" << endl;
		cout << "words in text: " << countWords << " pieces " << endl;
		break;
	case 3:
		unsigned short int len;
		cout << "lenght words  ";
		cin >> len;
		start_time = clock();
		length_word(root, len, "");
		end_time = clock();
		search_time = end_time - start_time;
		cout << "TIME length_word  " << search_time << " ms" << endl;
		break;
	case 4:
		for (string word = trie_get_first_word(root); word != ""; word = trie_get_next_word(-1, root, root, word)) 
		{
			cout << word << endl;
		}
		break;
	case 5:
		cout << "input index word: ";
		int input_index;
		cin >> input_index;
		for (string word = trie_get_first_word(root); word != ""; word = trie_get_next_word(-1, root, root, word)) 
		{
			if (input_index == 0) 
			{
				cout << word << endl;
				break;
			}
			input_index--;
		}
		break;

	}
	trie_free(root);
	
	return 0;
}
