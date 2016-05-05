#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

// Struktura koja cuva stop-rec i informaciju da li je ta stop-rec prva u novom redu
typedef struct {
	string word;
	bool changeLine;
} lineWord;

char lower(char c) { return tolower(c); }

string profiling(int n, string file_name)
{
	// Vektor 50 najcescih reci engleskog jezika
	vector<string> stop_words = { "the", "of", "and", "a", "in", "to", "is", "was", "it", "for", "with", "he", "be", "on", "i", "that", "by", "at", "you", "'s", "are", "not", "his", "this", "from", "but", "had", "which", "she", "they", "or", "an", "were", "we", "there", "been", "has", "have", "will", "would", "her", "n't", "there", "can", "all", "as", "if", "who", "what", "said" };
	vector<lineWord> words;
	lineWord line_word;
	vector<lineWord>::iterator it;
	string word;
	// Karakter "c" se koristi za proveru da li se u dokumentu doslo do nove linije
	char c;
	ifstream file;
	ofstream file_profile;
	int i = 0, j, l = 1;
	bool lineChanged = false, changeLine = false, emptyLine = true, doubleLine = false;
	
	// Otvaranje originalnog fajla
  file.open(file_name, ifstream::in);
  if (!file.is_open())
  {
		cout << "Error opening file: " << file_name << endl;
		exit (EXIT_FAILURE);
	}
  
  // Pravljenje fajla za upis profila
  size_t lastindex = file_name.find_last_of("."); 
	string file_name_profile = file_name.substr(0, lastindex); 
	file_name_profile = file_name_profile + "_profile.txt";
  file_profile.open(file_name_profile, ofstream::out);
  if (!file_profile.is_open())
	if (!file.is_open())
  {
		cout << "Error opening file: " << file_name_profile << endl;
		exit (EXIT_FAILURE);
	}

	// Izbacivanje reci koje nisu stop-reci i pravljenje n-grama
	// Prvih n reci
	while (file >> word >> c)
	{
		// Cita se jedan karakter posle reci, to je "c", zatim se dva karaktera
		// vracaju na stream, to su "c" i belina posle reci, zatim se ucitava "c" koji
		// sada predstavlja belinu posle reci. Tako detektujemo da li je u
		// dokumentu doslo do pojave novog reda.
		file.unget();
		file.unget();
		file.get(c);
		if (c == '\n')
		{
			// Ako linija ne sadrzi nijednu stop-rec
			if (emptyLine == true)
			{
				doubleLine = true;
				i = 0;
			}
				
			emptyLine = !emptyLine;
			// Usli smo u novu liniju
			lineChanged = true;
		}
			
		// Transformisanje velikih u mala slova kod reci
		transform(word.begin(), word.end(), word.begin(), lower);
    
    // Ako se rec nalazi u 50 najcescih stop-reci
		if (find(stop_words.begin(), stop_words.end(), word) != stop_words.end())
		{
			emptyLine = false;
			i++;
			
			// Preskakanje praznog niza i provera da li je trenutno procitana rec u novom redu
			if (words.size() > 2 && words.at(1).changeLine)
				changeLine = true;
			
			// Ako rec jeste u novom redu, brojac koji ispisuje broj red se inkrementira
			// Specijalno, ako je doslo do prazne linije, i bas do reci koja zapocinje liniju posle
			// nje (tj. brojac i je dosao do n), linija se uvecava za 2
			if (changeLine)
			{
				changeLine = false;
				if (doubleLine && i == n)
				{
					l += 2;
					doubleLine = false;
					i = 0;
				}
				else
					l++;
			}
			
			// Ako smo dosli do kraja reda upisujemo u sledecu stop-rec da je 
			// ona u novom redu
			line_word.word = word;
			if (c == '\n')
				line_word.changeLine = false;
			else
				line_word.changeLine = lineChanged;
			words.push_back(line_word);
				
			// Buduci da je novi red evidentiran, promenljivu koja ga oznacava postavljamo
			// na false
			// Specijalno, onemogucavamo inkrementiranje reda ako je stop-rec poslednja u redu
			if (lineChanged && c != '\n')
				lineChanged = false;
				
			file_profile << word << " ";
			
			// Upisali smo prvi n-gram i izlazimo iz petlje
			if (i == n)
				break;
		}
				
		// Karakter c se vraca na stream da ne bi poremetili citanje reci
		file.unget();
	}
	file_profile << l << endl;
	
	// Ostatak
	// Implementira se tako sto se reci siftuju za jedno mesto u levo, a na kraj se doda rec koja je 
	// upravo procitana. Ostatak je slican kao i za prvi n-gram
	while (file >> word >> c)
	{
		file.unget();
		file.unget();
		file.get(c);
		if (c == '\n')
		{
			if (emptyLine == true)
			{
				doubleLine = true;
				i = 0;
			}
				
			emptyLine = !emptyLine;
			lineChanged = true;
		}
		
		transform(word.begin(), word.end(), word.begin(), lower);
		
		if (find(stop_words.begin(), stop_words.end(), word) != stop_words.end())
		{			
			emptyLine = false;
			
			i++;
			
			if (words.at(1).changeLine)
				changeLine = true;
			
			if (changeLine)
			{
				changeLine = false;
				if (doubleLine && i == n)
				{
					l += 2;
					doubleLine = false;
					i = 0;
				}
				else
					l++;
			}
			
			words.erase(words.begin());
			
			line_word.word = word;
			if (c == '\n')
				line_word.changeLine = false;
			else
				line_word.changeLine = lineChanged;
			words.push_back(line_word);
				
			if (lineChanged && c != '\n')
				lineChanged = false;
				
			for (it = words.begin(); it != words.end(); ++it)
				file_profile << it->word << " ";

			file_profile << l << endl;
		}
		
		file.unget();
	}

  file.close();
  file_profile.close();
  
  return file_name_profile;
}

int member(string line)
{
	// Vektor 7 najcescih reci engleskog jezika
	vector<string> stop_words = { "the", "of", "and", "a", "in", "to", "'s" };
	int n = 0;
	
	// Parsiranje linije u vektor
	stringstream ss(line);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> test_words(begin, end);
	
	// Provera poklapanja date linije i vektora najcescih reci
	for (auto it = test_words.begin(); it != test_words.end(); ++it)
		if (find(stop_words.begin(), stop_words.end(), *it) != stop_words.end())
			n++;
	
	return n;
}

int maxseq(string line)
{
	// Vektor 7 najcescih reci engleskog jezika
	vector<string> stop_words = { "the", "of", "and", "a", "in", "to", "'s" };
	int max = 0, n = 0;
	
	// Parsiranje linije u vektor
	stringstream ss(line);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> test_words(begin, end);
	
	// Provera poklapanja date linije i vektora najcescih reci
	for (auto it = test_words.begin(); it != test_words.end(); ++it)
	{
		if (find(stop_words.begin(), stop_words.end(), *it) != stop_words.end())
		{
			n++;
			if (n > max)
				max = n;
		}
		else
			n = 0;
	}
	
	return max;
}

bool candidate(string original, string test_document, int n)
{
	ifstream file, test_file;
	string line1, line2;
	
	// Otvaranje originalnog i potencijalno plagijarizovanog dokumenta
	file.open(original, ifstream::in);
	if (!file.is_open())
  {
		cout << "Error opening file: " << original << endl;
		exit (EXIT_FAILURE);
	}
	test_file.open(test_document, ifstream::in);
	if (!test_file.is_open())
  {
		cout << "Error opening file: " << test_document << endl;
		exit (EXIT_FAILURE);
	}
	
	while(getline(file, line1))
	{
		while(getline(test_file, line2))
		{
			// Brisanje broja linije u kojoj se segment nalazi, koji je na kraju linije
			while (isdigit(line1.back()))
				line1.pop_back();
			while (isdigit(line2.back()))
				line2.pop_back();
				
			// Provera uslova koji plagirani dokument mora da ispuni
			if (line1 == line2)
				if (member(line1) < n-1 && maxseq(line1) < n-2)
					return true;
		}
		
		// Ponovno otvaranje drugog fajla jer smo dosli do njegovog kraja
		test_file.close();
		test_file.open(test_document, ifstream::in);
	}
	
	file.close();
  test_file.close();
	
	return false;
}

int main(int argc, char* argv[])
{
	string original, original_profile, line, lineNumbers;
	vector<string> documents, document_profiles, candidate_documents;
	map<int, int> M;
	ifstream file;
	int n = atoi(argv[1]), m = atoi(argv[2]);
	bool isFirst = true;
	
	// Otvaranje fajla u kome se nalaze imena svih dokumenata
	file.open(argv[3], ifstream::in);
	if (!file.is_open())
  {
		cout << "Error opening file: " << argv[3] << endl;
		exit (EXIT_FAILURE);
	}
	
	// Citanje imena originalnog dokumenta
	getline(file, line);
	original = line;
	
	// Citanje imena potencijalno plagijarizovanih dokumenata
	while (getline(file, line))
		documents.push_back(line);
	
	// Profilisanje dokumenata
	original_profile = profiling(n, original);
	for (auto it = documents.begin(); it != documents.end(); ++it)
		document_profiles.push_back(profiling(n, *it));
	
	// Odredjivanje kandidata
	for (auto it = document_profiles.begin(); it != document_profiles.end(); ++it)
	{
		if (candidate(original_profile, *it, n))
			candidate_documents.push_back(*it);
		else
		{
			// Ako dokument nije kandidat za plagijat
			// Odredjivanje imena koje je dokument imao pre profilisanja
			size_t lastindex = (*it).find_last_of("_"); 
			string file_name = (*it).substr(0, lastindex); 
			cout << endl << file_name << ".txt" << ": " << endl;
			cout << setw(20) << "Not plagiarism" << endl;
		}
	}
	
	return 0;
}
