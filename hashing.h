

union partdata{
	struct particle part;
	char data[20];	
};

char *SHA512_hash(char *data, int len);
void hashlist(partlist *parts);

