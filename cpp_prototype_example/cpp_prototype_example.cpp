#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>

const int N = 4;

//point struc
struct point
{
	int x, y;
};

std::vector <point> vdata;

// Abstract prototype
class Document
{

protected:
	std::string docname;
	Document(std::string c_docname)
	{
		docname = c_docname;
	}
public:
	virtual Document* clone(std::string name) const = 0;
	virtual void store() const = 0;
	virtual void record(std::vector<point> &vc) const = 0;
	virtual ~Document() { }
};

//concrete prototypes
class xmlDoc : public Document
{	
public:
	xmlDoc() :Document("") {};
	xmlDoc(std::string dname) :Document(dname) {};
	Document* clone(std::string name) const { return new xmlDoc(name); }
	void store() const { std::cout << "xmlDoc " << Document::docname.c_str() << "\n"; }
	void record(std::vector<point> &vc) const {
		std::ofstream fwriter(Document::docname.c_str());
		fwriter << "<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n";
		if (vc.size() > 0)
		{
			fwriter << "<x type=\"array\">\n";
			for (point p : vc)
			{
				fwriter << "<value>"<<p.x<<"</value>\n";
			}
			fwriter << "</x>\n";

			fwriter << "<y type=\"array\">\n";
			for (point p : vc)
			{
				fwriter << "<value>" << p.y << "</value>\n";
			}
			fwriter << "</y>\n";
		}
		fwriter.close();
	}
};

class plainDoc : public Document
{
public:
	plainDoc() :Document("") {};
	plainDoc(std::string dname) :Document(dname) {};
	Document* clone(std::string name) const { return new plainDoc(name); }
	void store() const { std::cout << "plainDoc " << Document::docname.c_str() << "\n"; }
	void record(std::vector<point> &vc) const {
		std::ofstream fwriter(Document::docname.c_str());
		fwriter << "VC output:\n";
		if (vc.size() > 0)
		{
			for (point p : vc)
			{
				fwriter << "x = " << p.x << "; y = "<< p.y << "\n";
			}			
		}
		fwriter.close();
	}
};

class spreadsheetDoc : public Document
{

public:
	spreadsheetDoc() :Document("") {};
	spreadsheetDoc(std::string dname) :Document(dname) {};
	Document* clone(std::string name) const { return new spreadsheetDoc(name); }
	void store() const { std::cout << "spreadsheetDoc " << Document::docname.c_str() << "\n"; }
	void record(std::vector<point> &vc) const {
		std::ofstream fwriter(Document::docname.c_str());
		if (vc.size() > 0)
		{
			for (point p : vc)
			{
				fwriter << p.x << ";" << p.y << "\n";
			}
		}
		fwriter.close();
	}
};


//doc manager class
class DocumentManager {
public:
	static Document* makeDocument(int choice, std::string dname);
	~DocumentManager() {}
private:
	static Document* mDocTypes[N];
};

//global list of document types
Document* DocumentManager::mDocTypes[] =
{
   0, new xmlDoc(), new plainDoc(), new spreadsheetDoc()
};


Document* DocumentManager::makeDocument(int choice, std::string dname)
{
	return mDocTypes[choice]->clone(dname);
}

//operator Destruct to remove objects
struct Destruct
{
	void operator()(Document *a) const { delete a; }
};


int main()
{
	//create data
	for (int i = 0; i < 10; i++)
	{
		vdata.push_back({ i,(int)pow(i,2) });
	}

	//holder of pointers to objects
	std::vector<Document*> docs = {};
	int choice;
	std::cout << "quit(0), xml(1), plain(2), spreadsheet(3): \n";

	while (true) 
	{
		std::cout << "Type in your choice (0-3)\n";
		std::cin >> choice;
		if (choice <= 0 || choice >= N)
			break;

		std::string dname;
		std::cout << "Input document name\n";
		std::cin >> dname;

		docs.push_back(DocumentManager::makeDocument(choice,dname));
	}

	for (int i = 0; i < docs.size(); ++i)
		if (docs[i])
		{
			docs[i]->store();
			docs[i]->record(vdata);
		}

	Destruct d;
	// this calls Destruct::operator()
	for_each(docs.begin(), docs.end(), d);

	//equivalent to:
	/*
	for (Document * cd : docs)
	{
		delete(cd);
	}
	*/

	return 0;

}