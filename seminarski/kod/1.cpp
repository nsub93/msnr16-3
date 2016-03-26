#include <iostream>

int main()
{
	// Inicijalizacija
	int suma = 0, i, n;
	std::cin >> n;
	
	for (i = 0; i < n; i++)
		suma += i;
	
	/* Stampanje sume */
	std::cout << "Suma: " << suma << std::endl;
	return 0;
}
