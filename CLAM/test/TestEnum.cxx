/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
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

#include "Enum.hxx"
#include <iostream>

using namespace CLAM;

/////////////////////////////////////////////////////////////////////
// Test Class
/////////////////////////////////////////////////////////////////////
namespace CLAMTest {
	class MyEnum : public Enum {
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		MyEnum() : Enum(sEnumValues, sDefault) {}
		MyEnum(tValue v) : Enum(sEnumValues, v) {};
		MyEnum(std::string s) : Enum(sEnumValues, s) {};
		virtual ~MyEnum() {};
		Component * Species() const { return new MyEnum;};

		typedef enum {
			zero=0,
			dos=2,
			cent=100
		} tEnum;
		static void TestClass ();
	};
	Enum::tEnumValue MyEnum::sEnumValues[] = {
		{MyEnum::zero,"zero"},
		{MyEnum::dos,"dos"},
		{MyEnum::cent,"cent"},
		{0,NULL}
	};

	Enum::tValue MyEnum::sDefault = MyEnum::dos;

	void MyEnum::TestClass () {
		std::cout << "-- Testing Enum" << std::endl;
		{
			std::cout << "+ Default constructor" << std::endl;
			MyEnum e;
			std::cout << e.GetString() << std::endl;
		}
		{
			std::cout << "+ Value constructor" << std::endl;
			MyEnum e(MyEnum::cent);
			std::cout << e.GetString() << std::endl;
		}
		{
			std::cout << "+ String constructor" << std::endl;
			MyEnum e("cent");
			std::cout << e.GetString() << std::endl;
		}
		{
			std::cout << "+ Value Set" << std::endl;
			MyEnum e;
			e.SetValue(0);
			std::cout << e.GetString() << std::endl;
		}
		{
			std::cout << "+ String Set" << std::endl;
			MyEnum e;
			e.SetValue("dos");
			std::cout << e.GetString() << std::endl;
		}
		{
			std::cout << "+ Illegal String Set" << std::endl;
			MyEnum e;
			try {
				e.SetValueSafely("dros");
				std::cout << e.GetString() << std::endl;
			} 
			catch (IllegalValue e) {
				std::cerr << "Exception thrown: " << e.msg << std::endl;
			}
		}
		{
			std::cout << "+ Illegal Value Set" << std::endl;
			MyEnum e;
			try {
				e.SetValueSafely(4);
				std::cout << e.GetString() << std::endl;
			} 
			catch (IllegalValue e) {
				std::cerr << "Exception thrown: " << e.msg << std::endl;
			}
		}
	}
}


int main () {
	CLAMTest::MyEnum::TestClass();
	return 0;
}


