#ifndef MiniCppUnit_hxx
#define MiniCppUnit_hxx

/**
 * miniCppUnit  (potser CppUnitLite ?)
 * (C) Oct 2003 Pau Arumi
 * 
 * @version 1.0 2003-10-28 Inicial
 * 
 * Exemple d'us:
 *
 * /code
 * class ElsMeusTests : public GrupDeTests<ElsMeusTests>
 * {
 *  public:
 *  	GRUP_DE_TESTS( TestosPrimeraIteracio )
 *		{
 *			CAS_DE_TEST( test );
 *			// etc
 *		}
 *		void test()
 *		{ 
 *			ASSERT_IGUALS( 4, 1+1+2 );
 *		}  
 * };
 * int main() { ElsMeusTests().testeja();  return 0; }
 *
 *
 * TODOs:
 *    
 *    - sortida consola fallades: enfatitzar "s'esperava", etc.
 *    - enfatitzar el punt on els strings difereixen.
 *    - Factoria per registrar GrupsDeTestos
 * 
 * Coses bones: 
 *
 *    - només són headers. No cal binari (compilar i linkar contra ell)
 *    - ús molt simple
 *    - assert d'Strings més currat que el de CppUnit
 *    - compatible amb Visual 6
 */

#include <iostream>
#include <string>
#include <sstream>
#include <list>
/*
#ifdef WIN32
//! la llibreria estàndar de Visual 6 no defineix 'min'
namespace std
{
	template<typename T>
	min(const T& a, const T& b) { return a < b ? a: b; }
}
#endif
*/
/**
 * Rep les resolucions dels casos de test i emmagatzema
 * el log d'errors per ser llistat al final.
 *
 * De moment és un singleton (una sola instancia permesa
 * amb accés global) per facilitar-ne l'accés desde els 
 * 'Asserts' però caldria refactoritzar perquè en comptes
 * d'accés global fos accés desde l'objecte GrupDeTests
 */
class EscoltadorDeTestos
{
private:
	//! constructor private per evitar-ne la instanciació forana
	EscoltadorDeTestos() : _nomTestActual(0) {}

public:
	static EscoltadorDeTestos& laInstancia()
	{
		static EscoltadorDeTestos instancia;
		return instancia;
	}

	std::stringstream& logErrors()
	{
		if (_nomTestActual)
			_log << "\nTest Fallat: " << (*_nomTestActual) << "\n";
		return _log;
	}

	std::string stringDelLog()
	{
		std::string aRetornar = _log.str();
		_log.str("");
		return aRetornar;

	}
	void nomTestActual( std::string& nom)
	{
		_nomTestActual = &nom;
	}
	static void nouOk()
	{
		std::cout << ".";
	}
	static void nouFallat()
	{
		std::cout << "F";
	}
	static void nouError()
	{
		std::cout << "E";
	}

private:
	
	std::string* _nomTestActual;
	std::stringstream _log;
};


/**
 * Classe abstracta que té la interfície per executar un test
 * és a dir: testesja() i nom()
 * és implementada per GrupDeTests i per CasDeTest
 *
 * Correspon al rol de 'Component' del patró 'Composite'
 **/
class Test
{
public:
	virtual ~Test(){}
	virtual void testeja() = 0;
	virtual std::string nom() const = 0;

};

/**
 * Classe base de la classe que implementa l'usuari amb
 * mètodes 'void f(void)' com a casos de test concret.
 *
 * Fa el rol de 'Composite' del patró 'Composite'
 * els seus 'fills' són objectes CasDeTest, els quals
 * wrappegen el mètodes casos de test.
 *
 * És un template del GrupDeTestsConcret perquè pugui
 * instanciar objectes CassDeTest templatitzats amb el mateix
 * parametre (en aquest cas el motiu és que es necessita el tipus 
 * de la classe per invocar-ne mètodes no estàtics). 
 */
template <typename GrupDeTestsConcret>
class GrupDeTests : public Test
{
protected:

	typedef GrupDeTestsConcret GrupConcret;
	typedef void(GrupDeTestsConcret::*MetodeDeTest)();

	/**
	 * Wrapper pels mètodes de test de les classes GrupsDeTest concretes
	 * de l'usuari.
	 * 
	 * Correspon al rol de 'Leave' del patró 'Composite' ja que són testos
	 * no compostos.
	 * També és un cas de patró 'Command' ja que encapsula l'execució de 
	 * certa funcionalitat en un objecte.
	 *
	 * És una classe aniuada (nested) a GrupDeTest ja que ...
	 * La creació d'instàncies de CasDeTest es realitza pel 
	 */
	class CasDeTest : public Test
	{
	public:
		CasDeTest(GrupConcret* pare, MetodeDeTest metode, const char* nom) :
		  _pare(pare),
		  _metodeDeTest(metode),
		  _nom(nom)
		{
		}

		void testeja()
		{
			EscoltadorDeTestos::laInstancia().nouOk();
			EscoltadorDeTestos::laInstancia().nomTestActual(_nom);
			try
			{
				(_pare->*_metodeDeTest)();
			}
			catch( std::exception& error )
			{
				EscoltadorDeTestos::laInstancia().nouError();
				EscoltadorDeTestos::laInstancia().logErrors() << "std::exception caçada al MiniCppUnit: "
				<< error.what() << "\n";
			}
			catch(...)
			{
				EscoltadorDeTestos::laInstancia().nouError();
				EscoltadorDeTestos::laInstancia().logErrors() << "Excepcio no identificada.\n";
			}
		}

		std::string nom() const
		{
			return _nom;
		}

	private:
		GrupConcret* _pare;
		MetodeDeTest _metodeDeTest;
		std::string _nom;
	};
    //------------- fi de class CasDeTest ----------------------------

private:
	
	typedef std::list<Test*> CasosDeTest;
	CasosDeTest _casosDeTest;
	std::string _nom;

	void llistaTests() const
	{
		std::cout << "+ " << nom() << "\n";
		for( CasosDeTest::const_iterator it=_casosDeTest.begin(); 
			it!=_casosDeTest.end(); it++ )
			std::cout << "  - "<< (*it)->nom() << "\n";

		std::cout << "\n" << "Total " << int(_casosDeTest.size()) << " tests.\n\n";
	}
	

public:
	virtual void setUp() {}
	virtual void tearDown() {}

	std::string nom() const 
	{
		return _nom;
	};

	GrupDeTests(const std::string& nom="Un grup de tests") : _nom(nom)
	{
	}

	void afegeixCasDeTest(GrupConcret* pare, MetodeDeTest metode, const char* nom)
	{
		CasDeTest* casDeTest = new CasDeTest(pare, metode, nom);
		_casosDeTest.push_back( casDeTest );
	}

	void testeja()
	{
		llistaTests();

		for( CasosDeTest::iterator it=_casosDeTest.begin(); it!=_casosDeTest.end(); it++)
		{
			setUp();
			(*it)->testeja();
			tearDown();
		}
		std::cout << "\n" << EscoltadorDeTestos::laInstancia().stringDelLog();
	}
	~GrupDeTests()
	{
		for( CasosDeTest::iterator it=_casosDeTest.begin(); it!=_casosDeTest.end(); it++)
            delete (*it);
	}
};

//----------------------------------------------------------

/**
 * Classe que conté els diferents tipus d'asserts com a funcions estàtiques
 * Està pensat perquè l'usuari les utilitzi fent servir les macros del final
 */
class Assert
{
private:
	static std::string partComuna(const std::string& str1, const std::string& str2)
	{
		std::string::size_type pos=0;
		std::string::const_iterator it1 = str1.begin();
		std::string::const_iterator it2 = str2.begin();
        while( it1!=str1.end() && it2!=str2.end() )
		{
			if ( *(it1) != *(it2) )
				return std::string(str1, 0, pos);
			it1++;
			it2++;
			pos++;
		}
		return str1.size() < str2.size() ? str1 : str2;
	}

public:
	static void assertTrue(char* strExpressio, bool expressio,
			const char* fitxer="", int linia=0)
	{
		if (!expressio)
		{
			EscoltadorDeTestos::laInstancia().logErrors() <<
				"\nTest fallat a " << fitxer << ", linia: " << linia <<
				"\nNo s'ha complert: "  << strExpressio << "\n";

			EscoltadorDeTestos::laInstancia().nouFallat();
		}
	}

	static void assertTrueMissatge(char* strExpressio, bool expressio, const char* missatge,
			const char* fitxer="", int linia=0)
	{
		if (!expressio)
		{
			EscoltadorDeTestos::laInstancia().logErrors() <<
				"\nTest fallat a " << fitxer << ", linia: " << linia <<
				"\nNo s'ha complert: "  << strExpressio <<
				"\n"<< missatge<<"\n";

			EscoltadorDeTestos::laInstancia().nouFallat();
		}
	}


	template<typename UnTipus>
	static void assertIguals( const UnTipus& esperat, const UnTipus& resultat,
		const char* fitxer="", int linia=0 )
	{
		if(esperat != resultat)
		{
			EscoltadorDeTestos::laInstancia().logErrors() <<
				fitxer << ", linia: " << linia <<
				"\nS'esperava: \n" << esperat << "\npero s'ha trobat: \n" << resultat << "\n";

			EscoltadorDeTestos::laInstancia().nouFallat();
		}
	}

	static int notEqualIndex( const std::string & one, const std::string & other )
	{
		int end = std::min(int(one.length()), int(other.length()));
		for ( int index = 0; index < end; index++ )
			if (one[index] != other[index] )
				return index;
		return end;
	}

	/**
	 * sobrecarreguem la versió d'arguments string
	 * per tal de donar una ajuda del punt on es diferencien.
	 *
	 * Visual 6 no deixa que els strings siguin per referència :-(
	 */
	static void assertIguals( const std::string esperat, const std::string resultat,
		const char* fitxer="", int linia=0 )
	{
		if(esperat != resultat)
		{
			int indexDiferent = notEqualIndex(esperat, resultat);
			EscoltadorDeTestos::laInstancia().logErrors()
				<< fitxer << ", linia: " << linia << "\n"
				<< "S'esperava: \n\033[36;1m" << esperat.substr(0,indexDiferent)
				<< "\033[32;1m" << esperat.substr(indexDiferent) << "\033[0m\n"
				<< "Pero s'ha trobat:\033[36;1m \n" << resultat.substr(0,indexDiferent)
				<< "\033[31;1m" << resultat.substr(indexDiferent) << "\033[0m\n";
//				<< " Posicio: " << indexDiferent << "\n";

			EscoltadorDeTestos::laInstancia().nouFallat();
		}
	}
	//! retorna si el test ha passat. En aquest cas sempre fals
	static void falla(const char* motiu, const char* fitxer="", int linia=0)
	{
		EscoltadorDeTestos::laInstancia().logErrors() <<
			fitxer << ", linia: " << linia << "\n" <<
			motiu << "\n";

		EscoltadorDeTestos::laInstancia().nouFallat();
	}

};

/**
 * Macros d'assert a usar dins dels mètodes de testeig.
 * Un assert és una comprovació que volem testejar.
 */
#define ASSERT_IGUALS( esperat, resultat) \
	Assert::assertIguals( esperat, resultat, __FILE__, __LINE__ );

#define ASSERT( exp ) \
	Assert::assertTrue(#exp, exp, __FILE__, __LINE__);

#define ASSERT_MISSATGE( exp, missatge ) \
	Assert::assertTrueMissatge(#exp, exp, missatge, __FILE__, __LINE__);


#define FALLA( motiu ) \
	Assert::falla(#motiu, __FILE__, __LINE__);

/**
 * Macros que faciliten l'escriptura del constructor de la classe
 * amb mètodes de test. Tant senzill com això:
 *
 * /code
 * class ElsMeusTests : public GrupDeTests<ElsMeusTests>
 * {
 *  public:
 *  	GRUP_DE_TESTS( TestosPrimeraIteracio )
 *		{
 *			CAS_DE_TEST( test );
 *			// etc
 *		}
 *		void test()
 *		{
 *			ASSERT_IGUALS( 4, 1+1+2 );
 *		}
 */

#define GRUP_DE_TESTS( GrupTestsConcret ) \
	GrupTestsConcret() : GrupDeTests< GrupConcret >( #GrupTestsConcret )

#define CAS_DE_TEST( nomDelMetode ) \
	afegeixCasDeTest( this, &GrupConcret::nomDelMetode, #nomDelMetode );

#endif  // MiniCppUnit_hxx
