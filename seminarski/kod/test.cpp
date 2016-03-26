#include <iostream>


int main()
{
	// Promenjen komentar
	int suma=0, i, n;
	std::cin >> n;
	
	
	for (i=0; i<n; i++)
      suma+=i;
	
	
	/* Jos jedan promenjen komentar */
	std::cout << "Suma: " << suma << std::endl;
	
	return 0;
}
