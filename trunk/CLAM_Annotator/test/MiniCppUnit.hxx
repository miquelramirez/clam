/*
 * Copyright (c) 2003-2004  Pau Arum� & David Garc�a
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef MiniCppUnit_hxx
#define MiniCppUnit_hxx

/**
 * miniCppUnit  (potser CppUnitLite ?)
 * (C) Oct 2003 Pau Arumi
 * 
 * @version 2.1 2004-11-04 Especialitzacio char*
 * @version 2.0 2004-10-26 Inicial
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
 *
 * REGISTRA_TEST(ElsMeusTests);
 * 
 * int main()
 * {
 * 	ElsMeusTests().testeja();
 * 	return 0;
 * }
 *
 *
 * Coses bones: 
 *
 *    - nom�s s�n headers. No cal binari (compilar i linkar contra ell)
 *    - �s molt simple
 *    - assert d'Strings m�s currat que el de CppUnit
 *    - compatible amb Visual 6
 */

#include <iostream>
#include <string>
#include <sstream>
#include <list>

#ifdef WIN32
//! la llibreria est�ndar de Visual 6 no defineix 'min'
namespace std
{
	template<typename T>
	min(const T& a, const T& b) { return a < b ? a: b; }
}
#endif

/**
 * Rep les resolucions dels casos de test i emmagatzema
 * el log d'errors per ser llistat al final.
 *
 * De moment �s un singleton (una sola instancia permesa
 * amb acc�s global) per facilitar-ne l'acc�s desde els 
 * 'Asserts' per� caldria refactoritzar perqu� en comptes
 * d'acc�s global fos acc�s desde l'objecte GrupDeTests
 */
class EscoltadorDeTestos
{
private:
	//! constructor private per evitar-ne la instanciaci� forana
	EscoltadorDeTestos() : _nomTestActual(0)
	{
		_executats=_fallades=_excepcions=0;
	}

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
	static void nouExecutat()
	{
		std::cout << ".";
		laInstancia()._executats++;
	}
	static void nouFallat()
	{
		std::cout << "F";
		laInstancia()._fallades++;
	}
	static void nouError()
	{
		std::cout << "E";
		laInstancia()._excepcions++;
	}
	std::string summary()
	{
		std::ostringstream os;
		os	<< "\nSumari:\n"
			<< "\033[" "1m\tTest Executats:         " << _executats << "\033[0m\n"
			<< "\033[32;1m\tTest Passats:           " << (_executats-_fallades-_excepcions) << "\033[0m\n";
		if (_fallades > 0)
		{
			os 
				<< "\033[31;1m\tTest Fallats:           " 
				<< _fallades << "\033[0m\n";
		}
		if (_excepcions > 0)
		{
			os 
				<< "\033[33;1m\tExcepcions inesperades: " 
				<< _excepcions << "\033[0m\n";
		}
		os << std::endl;
		return os.str();
	}
	static bool passaTotsElsTests()
	{
		return !laInstancia()._excepcions && !laInstancia()._fallades;
	}

private:
	
	std::string* _nomTestActual;
	std::stringstream _log;
	unsigned _executats,_fallades,_excepcions;
};


/**
 * Classe abstracta que t� la interf�cie per executar un test
 * �s a dir: testesja() i nom()
 * �s implementada per GrupDeTests i per CasDeTest
 *
 * Correspon al rol de 'Component' del patr� 'Composite'
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
 * m�todes 'void f(void)' com a casos de test concret.
 *
 * Fa el rol de 'Composite' del patr� 'Composite'
 * els seus 'fills' s�n objectes CasDeTest, els quals
 * wrappegen el m�todes casos de test.
 *
 * �s un template del GrupDeTestsConcret perqu� pugui
 * instanciar objectes CassDeTest templatitzats amb el mateix
 * parametre (en aquest cas el motiu �s que es necessita el tipus 
 * de la classe per invocar-ne m�todes no est�tics). 
 */
template <typename GrupDeTestsConcret>
class GrupDeTests : public Test
{
protected:

	typedef GrupDeTestsConcret GrupConcret;
	typedef void(GrupDeTestsConcret::*MetodeDeTest)();

	/**
	 * Wrapper pels m�todes de test de les classes GrupsDeTest concretes
	 * de l'usuari.
	 * 
	 * Correspon al rol de 'Leave' del patr� 'Composite' ja que s�n testos
	 * no compostos.
	 * Tamb� �s un cas de patr� 'Command' ja que encapsula l'execuci� de 
	 * certa funcionalitat en un objecte.
	 *
	 * �s una classe aniuada (nested) a GrupDeTest ja que ...
	 * La creaci� d'inst�ncies de CasDeTest es realitza pel 
	 */
	class CasDeTest : public Test
	{
	public:
		CasDeTest(GrupConcret* pare, MetodeDeTest metode, const std::string & nom) : 
		  _pare(pare),
		  _metodeDeTest(metode),
		  _nom(nom)
		{
		}

		void testeja()
		{
			EscoltadorDeTestos::laInstancia().nouExecutat();
			EscoltadorDeTestos::laInstancia().nomTestActual(_nom);
			try
			{
				(_pare->*_metodeDeTest)();
			}
			catch( std::exception& error )
			{
				EscoltadorDeTestos::laInstancia().nouError();
				EscoltadorDeTestos::laInstancia().logErrors() << "std::exception ca�ada al MiniCppUnit: "
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
		std::cout << "\n+ " << nom() << "\n";
		for( CasosDeTest::const_iterator it=_casosDeTest.begin(); 
			it!=_casosDeTest.end(); it++ )
			std::cout << "  - "<< (*it)->nom() << "\n";
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
		CasDeTest* casDeTest = new CasDeTest(pare, metode, _nom + "::" + nom);
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
	}
	~GrupDeTests()
	{
		for( CasosDeTest::iterator it=_casosDeTest.begin(); it!=_casosDeTest.end(); it++)
			delete (*it);
	}
};

//----------------------------------------------------------

/**
 * Classe que cont� els diferents tipus d'asserts com a funcions est�tiques
 * Est� pensat perqu� l'usuari les utilitzi fent servir les macros del final
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

	static void assertIguals( const char * esperat, const char * resultat,
		const char* fitxer="", int linia=0 )
	{
		assertIguals(std::string(esperat), std::string(resultat),
			fitxer, linia);
	}
	static void assertIguals( const bool& esperat, const bool& resultat,
		const char* fitxer="", int linia=0 )
	{
		assertIguals(
			(esperat?"true":"false"), 
			(resultat?"true":"false"),
			fitxer, linia);
	}

	static int notEqualIndex( const std::string & one, const std::string & other )
	{
		int end = std::min(one.length(), other.length());
		for ( int index = 0; index < end; index++ )
			if (one[index] != other[index] )
				return index;
		return end;
	}

	/**
	 * sobrecarreguem la versi� d'arguments string
	 * per tal de donar una ajuda del punt on es diferencien.
	 *
	 * Visual 6 no deixa que els strings siguin per refer�ncia :-(
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
			"Motiu: " << motiu << "\n";

		EscoltadorDeTestos::laInstancia().nouFallat();
	}

};

/**
 * Classe destinada a guardar registre de totes les classes de tests
 */
class FactoryDeTests
{
private:
	//! Don't allow instantiation apart from theInstance()
	FactoryDeTests()
	{
	}
	typedef Test* (*CreadorTest)();
	std::list<CreadorTest> _creadors;
public:
	static FactoryDeTests& laInstancia()
	{
		static FactoryDeTests laFactory;
		return laFactory;
	}
	bool testejaTots()
	{
		std::list<CreadorTest>::iterator it;
		for(it=_creadors.begin(); it!=_creadors.end(); it++)
		{	
			CreadorTest creador = *it;
			Test* test = creador();
			test->testeja();
			delete test;
		}
		std::string errors =  EscoltadorDeTestos::laInstancia().stringDelLog();
		if (errors!="") std::cout << "\n\nDetall d'errors:\n" << errors;
		std::cout << EscoltadorDeTestos::laInstancia().summary();

		return EscoltadorDeTestos::laInstancia().passaTotsElsTests();	
	}
	void afegeixCreadorDeTests(CreadorTest crea)
	{
		_creadors.push_back( crea );
	}
	
};

/** 
 * Macro a usar despr�s de cada classe de test
 */
#define REGISTRA_TEST( ClasseDeTest ) \
\
Test* Creador##ClasseDeTest() { return new ClasseDeTest; } \
\
class Registrador##ClasseDeTest \
{ \
public: \
	Registrador##ClasseDeTest() \
	{ \
		FactoryDeTests::laInstancia().afegeixCreadorDeTests( \
				Creador##ClasseDeTest); \
	} \
}; \
static Registrador##ClasseDeTest estatic##ClasseDeTest;






/**
 * Macros d'assert a usar dins dels m�todes de testeig.
 * Un assert �s una comprovaci� que volem testejar.
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
 * amb m�todes de test. Tant senzill com aix�:
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
	GrupTestsConcret() : GrupDeTests<GrupConcret>( #GrupTestsConcret )

#define CAS_DE_TEST( nomDelMetode ) \
	afegeixCasDeTest( this, &GrupConcret::nomDelMetode, #nomDelMetode );

#endif  // MiniCppUnit_hxx
