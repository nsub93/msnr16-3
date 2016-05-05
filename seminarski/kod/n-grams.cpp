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

int getLines(string original)
{
	ifstream file;
	string line;
	int n = 0;
	
	// Otvaranje originalnog dokumenta
	file.open(original, ifstream::in);
	if (!file.is_open())
  {
		cout << "Error opening file: " << original << endl;
		exit (EXIT_FAILURE);
	}
	
	while(getline(file, line))
		n++;
	
	return n;
}

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

map<int, int> mapping(string original, string test_document)
{
	ifstream file, test_file;
	string line1, line2;
	map<int, int> M;
	// Brojaci koji sluze da znamo na kojoj se liniji u fajlu nalazimo
	int i = 1, j = 1;
	
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
	
	// Poredjenje linija u dokumentima
	while(getline(file, line1))
	{
		while(getline(test_file, line2))
		{
			// Brisanje broja linije u kojoj se segment nalazi, koji je na kraju linije
			while (isdigit(line1.back()))
				line1.pop_back();
			while (isdigit(line2.back()))
				line2.pop_back();
			
			// Brojevi parova linija koje se poklapaju se unose u mapu
			if (line1 == line2)
				M.insert(make_pair(i, j));
			j++;
		}
		
		// Ponovno otvaranje drugog fajla jer smo dosli do njegovog kraja
		test_file.close();
		test_file.open(test_document, ifstream::in);
		
		// Resetovanje brojaca
		j = 1;
		i++;
	}
	
	file.close();
	test_file.close();
	
	return M;
}

map<int, int> boundary(string original, string test_document, int m)
{
	// Mapiranje parova linija u originalnom i plagiranom dokumentu koje se poklapaju
	map<int, int> M = mapping(original, test_document), M1;
	// Element koji pamti pocetak plagijarozovanog segmenta
	map<int, int>::iterator begin = M.begin();
	map<int, int>::iterator it;
	
	for (it = M.begin(); it != M.end(); ++it)
	{
		// Provera razlike izmedju dva uzastopna segmenta
		if ((++it)->first - (--it)->first > m || (++it)->second - (--it)->second > m)
		{
			// Ubacivanje granica segmenta, tj. pocetak i par do kog smo stigli
			M1.insert(make_pair(begin->first, begin->second));
			M1.insert(make_pair(it->first, it->second));
			// Kao novi pocetak postavlja se sledeci par
			begin = (++it);
			// Iterator se vraca jedno mesto u nazad
			--it;
		}
	}
	// Stiglo se do kraja petlje, znaci da treba da se ubaci gornja granica poslednjeg segmenta
	M1.insert(make_pair(begin->first, begin->second));
	M1.insert(make_pair(it->first, (++it)->second));
	
	return M1;
}

int char_profiling(int n, int x1, int x2, string file_name)
{
	vector<char> chars;
	char c;
	string line;
	// Promenljiva "i" koristi se za vracanje broja linija rezultujuceg fajla
	// Promenljiva "k" koristi se za pracenje broja linija originalnog fajla
	int i = 0, j, k = 1;
	ifstream file;
	ofstream file_profile;
  
  file.open(file_name, ifstream::in);
  if (!file.is_open())
  {
		cout << "Error opening file: " << file_name << endl;
		exit (EXIT_FAILURE);
	}
	
  // Pomeramo se na pocetak segmenta
  // Specijalno, preskacemo uzimanje prve linije, ako plagiranje krece bas od nje
  if (x1 > 1)
  {
		while(getline(file, line))
		{
			if (k == x1-1)
				break;
			k++;
		}
	}
	else
		k = 1;
  
  // Pravljenje fajla za upis karakterskog profila
  size_t lastindex = file_name.find_last_of("_"); 
	string file_name_profile = file_name.substr(0, lastindex); 
	file_name_profile = file_name_profile + "_char_profile.txt";
  file_profile.open(file_name_profile, ofstream::out);
  if (!file_profile.is_open())
  {
		cout << "Error opening file: " << file_name_profile << endl;
		exit (EXIT_FAILURE);
	}

	// Upisivanje prvih n karaktera
	while(file.get(c))
	{
		i++;
		chars.push_back(c);
		file_profile << c;
		if (i == n)
			break;
	}
	file_profile << endl;
	i = 1;
	
	// Ostatak
	// Implementira se tako sto se karakteri siftuju za jedno mesto u levo, a na kraj se doda karakter koji je 
	// upravo procitan
	while (file.get(c))
	{
		if (c == '\n')
		{
			k++;
			// Zaustavljamo se na kraju segmenta
			if (k == x2+1)
				break;
			continue;
		}
		// Preskakanje broja linije koji se nalazi na kraju
		else if (isdigit(c))
			continue;
		
		i++;
		for (j = 1; j < n; j++)
			chars[j-1] = chars[j];
		chars[j-1] = c;
		for (j = 0; j < n; j++)
			file_profile << chars[j];
		file_profile << endl;
	}
	
	file.close();
	file_profile.close();
	
	return i;
}

int match(string dx, string ds)
{
	ifstream file_profile1, file_profile2;
  string line1, line2;
  int n = 0;
  
  // Iscitavanje fajlova sa karakterskim profilima
  size_t lastindex1 = dx.find_last_of("_"); 
	string file_name_profile1 = dx.substr(0, lastindex1); 
	file_name_profile1 = file_name_profile1 + "_char_profile.txt";
  file_profile1.open(file_name_profile1, ifstream::in);
  if (!file_profile1.is_open())
  {
		cout << "Error opening file: " << file_name_profile1 << endl;
		exit (EXIT_FAILURE);
	}
  
  size_t lastindex2 = ds.find_last_of("_"); 
	string file_name_profile2 = ds.substr(0, lastindex2); 
	file_name_profile2 = file_name_profile2 + "_char_profile.txt";
  file_profile2.open(file_name_profile2, ifstream::in);
  if (!file_profile2.is_open())
  {
		cout << "Error opening file: " << file_name_profile2 << endl;
		exit (EXIT_FAILURE);
	}

	// Brojanje podudarnih linija
	while(getline(file_profile1, line1))
	{
		while(getline(file_profile2, line2))
		{
			// Brisanje broja linije u kojoj se segment nalazi, koji je na kraju linije
			while (isdigit(line1.back()))
				line1.pop_back();
			while (isdigit(line2.back()))
				line2.pop_back();
			if (line1 == line2)
			{
				n++;
				break;
			}
		}
		
		// Ponovno otvaranje drugog fajla jer smo dosli do njegovog kraja
		file_profile2.close();
		file_profile2.open(file_name_profile2, ifstream::in);
	}
	
	file_profile1.close();
	file_profile2.close();
	
	return n;
}

float similarity(int n, int x1, int y1, string dx, int x2, int y2, string ds)
{
	int matches = 0, x, y;
	
	// Pravljenje karakterskih profila dokumenata, ali samo u okviru segmenata
	// koji su dobijeni kao argumenti funkcije
	x = char_profiling(n, x1, x2, dx);
	y = char_profiling(n, y1, y2, ds);
	
	// Poredjenje poklapanja linija karakterskih profila originalnog i 
	// plagijarizovanog dokumenta
	matches = match(dx, ds);
	
	// Racunanje i vracanje koeficijenta slicnosti
	return ((float)matches)/(max(x, y));
}

string getLineNumbers(int x1, int x2, string file_name)
{
	ifstream file;
	string line, result;
	// Promenljiva "k" koristi se za pracenje broja linije fajla u
	// kojoj se nalazimo
	int k = 1, x, y = 1, z = 0;
	
	file.open(file_name, ifstream::in);
	if (!file.is_open())
  {
		cout << "Error opening file: " << file_name << endl;
		exit (EXIT_FAILURE);
	}
	
	// Pomeramo se na pocetak segmenta
  while(getline(file, line))
	{
		if (k == x1)
			break;
		k++;
	}
	
	result = "(";
	// Citamo broj linije na kojoj pocinje n-gram u originalnom dokumentu
	// Buduci da citamo sa desna na levo, moramo da obrnemo procitani broj
	// Upisivanje broja pocetne linije segmenta
	while (isdigit(line.back()))
	{
		x = line.back()-48;
		x *= y;
		y *= 10;
		z += x;
		line.pop_back();
	}
	result += to_string(z);
	y = 1;
	z = 0;
	k++;
	
	while (getline(file, line))
	{
		// Provera da li smo dosli do kraja segmenta
		if (k == x2)
		{
			result += ", ";
			// Upisivanje broja krajnje linije segmenta
			while (isdigit(line.back()))
			{
				x = line.back()-48;
				x *= y;
				y *= 10;
				z += x;
				line.pop_back();
			}
			result += to_string(z);
			break;
		}
		k++;
	}
	result += ")";
	
	file.close();
	
	return result;
}

int main(int argc, char* argv[])
{
	string original, original_profile, line, lineNumbers;
	vector<string> documents, document_profiles, candidate_documents;
	map<int, int> M;
	ifstream file;
	int n = atoi(argv[1]), m = atoi(argv[2]), lines, x1, y1, x2, y2, z1, z2;
	float similarityValue, overallSimilarity = 0;
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
	
	// Odredjivanje broja linija orginalnog dokumenta
	lines = getLines(original);
	
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
	
	// Odredjivanje koreficijenta plagijarizma svakog od plagijarizovanih segmenata
	for (auto it = candidate_documents.begin(); it != candidate_documents.end(); ++it)
	{
		// Odredjivanje granica segmenata
		M = boundary(original_profile, *it, m);
		
		// Odredjivanje imena koje je dokument imao pre profilisanja
		size_t lastindex = (*it).find_last_of("_"); 
		string file_name = (*it).substr(0, lastindex); 
		cout << endl << file_name << ".txt" << ": " << endl;
		cout << setw(12) << "Original" << setw(12) << "Plagiat" 
			<< setw(27) << "Plagiarism coefficient" << setw(27) << "Similarity percentage" << endl;
		for (auto jt = M.begin(); jt != M.end(); ++jt)
		{		
			string z1_str, z2_str;
			
			// Ucitava se par, pocetak i kraj, segmenta
			x1 = jt->first;
			y1 = jt->second;
			jt++;
			x2 = jt->first;
			y2 = jt->second;
			
			// Odredjivanje granica segmenta u originalnom dokumentu
			lineNumbers = getLineNumbers(x1, x2, original_profile);
			cout << setw(12) << lineNumbers;
			
			for (int l = 0; l < lineNumbers.size(); l++)
			{
				if (lineNumbers[l] == ',')
					isFirst = false;
				
				if (isFirst)
				{
					if (isdigit(lineNumbers[l]))
						z1_str += (lineNumbers[l]);
				}
				else
				{
					if (isdigit(lineNumbers[l]))
						z2_str += (lineNumbers[l]);
				}
			}
			isFirst = true;
			
			z1 = stoi(z1_str);
			z2 = stoi(z2_str);
			
			// Odredjivanje granica segmenta u plagiranom dokumentu
			lineNumbers = getLineNumbers(y1, y2, *it);
			cout << setw(12) << lineNumbers;
			
			// Odredjivanje slicnosti segmenta u originalnom i plagiranom dokumentu 
			similarityValue = similarity(n, x1, y1, original_profile, x2, y2, *it);
			cout << setw(16) << similarityValue;
			cout << setw(28) << similarityValue*100 << setw(1) << "%" << endl;
			
			// Izracunavanje procenta originalnog fajla koji je plagiran
			overallSimilarity += z2 - z1;
		}
		cout << endl << setw(24) << "Overall similarity: " << ((overallSimilarity+5)/lines)*100 << setw(1) << "%" << endl;
		overallSimilarity = 0;
	}
	cout << endl;
	
	return 0;
}
