int Mas[10];
unsigned long i;
int *pMas = Mas;
int main() {
	for (i = 0; i < 9; i++) {
		if (i != 6)
			*(pMas + i) = (17*i) & 0x0E;
		else {
 			*(pMas + i) = 0x1A * i/4;
 		}
	}
 	return 0;
}