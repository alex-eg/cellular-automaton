class set {
private:
    unsigned char cont[8];
    unsigned char size;
public:
    set();
    void add(unsigned char val);
    bool in(unsigned char val);
    bool del(unsigned char val);
    set &operator = (const set &right)
    {
	if (this == &right)
	    return *this;
	size = right.size;
	for (int i=0; i<size; i++)
	    cont[i] = right.cont[i];
	return *this;
    }
};
