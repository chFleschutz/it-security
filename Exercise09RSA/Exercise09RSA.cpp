#include <random>
#include <iostream>

struct RSAKeyPair
{
	int n;
	int pub;
	int priv;
};

int euclidean(int a, int b)
{
	while (b != 0)
	{
		auto tmp = b;
		b = a % b;
		a = tmp;
	}

	return a;
}

std::tuple<int, int, int> extendedEuclidean(int a, int b)
{
	if (b == 0)
		return { a, 1, 0 };

	auto&& [g, a2, b2] = extendedEuclidean(b, a % b);
	return { g, b2, a2 - (b2 * (a / b)) };
}

bool isPrime(int x)
{
	if (x < 2)
		return false;

	int rootX = sqrt(x);
	for (int i = 2; i <= rootX; i++)
	{
		if (x % i == 0)
			return false;
	}

	return true;
}

int modPower(long long base, int exponent, int modulo)
{
	if (modulo == 1)
		return 0; // Modulo 1 is undefined

	long long result = 1;
	base = base % modulo;

	while (exponent > 0) 
	{
		// If exponent is odd, multiply base with result
		if (exponent % 2 == 1) 
		{
			result = (result * base) % modulo;
		}

		// exponent must be even now
		exponent = exponent >> 1; // Equivalent to exponent /= 2
		base = (base * base) % modulo;
	}

	return static_cast<int>(result); // is always in int range due to modulo being an int
}

RSAKeyPair genRSAKeypair()
{
	// Init Random generator
	std::random_device rd;
	std::mt19937 rng(rd());

	constexpr int maxNum = 46340;

	// N 
	int p = 0;
	while (!isPrime(p))
		p = rng() % maxNum;

	int q = 0;
	while (!isPrime(q))
		q = rng() % maxNum;

	int n = p * q; 

	// Public key
	int phi = (p - 1) * (q - 1);
	int a = 0;
	while (!isPrime(a) or euclidean(a, phi) != 1)
		a = rng() % phi;

	// Private key
	auto [g, b, _] = extendedEuclidean(a, phi);
	b = (b % phi + phi) % phi; // ensure private key is positive

	return RSAKeyPair{ n, a, b };
}

void print(const RSAKeyPair& key)
{
	std::cout << "RSAKeyPair: " << "\n";
	std::cout << "  n:       " << key.n << "\n";
	std::cout << "  public:  " << key.pub << "\n";
	std::cout << "  private: " << key.priv << "\n";
}

int encode(int message, int pub, int n)
{
	return modPower(message, pub, n);
}

int decode(int chiffre, int priv, int n)
{
	return modPower(chiffre, priv, n);
}

int main()
{
	int message = 42;

	auto key = genRSAKeypair();
	int chiffre = encode(message, key.pub, key.n);
	int decoded = decode(chiffre, key.priv, key.n);

	print(key);

	std::cout << "Message: " << message << "\n";
	std::cout << "Chiffre: " << chiffre << "\n";
	std::cout << "Decoded: " << decoded << "\n";

	if (decoded == message)
		std::cout << "RSA works :)\n";
	else
		std::cout << "What is this ???\n";
}
