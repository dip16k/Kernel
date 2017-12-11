#define TYPE 'Z'
#define FIOP _IO(TYPE,0)
#define FIOGETCONFIG _IOR(TYPE,1,int)
#define FIOSETCONFIG _IOW(TYPE,2,int)
#define FIOCLEAR _IOC(TYPE,3,int)

static struct mystatistics {
		int Baud;
		int nBits;
		int nr;
		int nw;
		char buff[10];
};

