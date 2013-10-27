

union partdata{
	struct particle part;
	char data[20];	
};
union num{
	char bytes[4];
	uint32_t val;
};


char *SHA512_hash(char *data, int len);
