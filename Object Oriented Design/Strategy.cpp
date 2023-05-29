#include <string>

class Printer
{
public:
	virtual void doPrint(std::string document) = 0;
};

class PdfPrinter : public Printer
{
public:
	void doPrint(std::string document) override { }
};

class RealPrinter : public Printer
{
public:
	void doPrint(std::string document) override { }
};

class IPrinterStrategy
{
public:
	virtual void doPrint(std::string document) = 0;
};


class PrintSystem
{
public:
	void Print(std::string document, IPrinterStrategy* strategy)
	{
		// Do some
		strategy->doPrint(document);
		// Do something else
	}
};

class PdfStrategy : public IPrinterStrategy
{
public:
	void doPrint(std::string document) override { }
};

class RealStrategy : public IPrinterStrategy
{
public:
	void doPrint(std::string document) override { }
};