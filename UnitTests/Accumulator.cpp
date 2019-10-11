#include "pch.h"
#include "CppUnitTest.h"
#include "../WTest_OrderBook/Accumulator.h"
#include "Utils.h"
using namespace WG_ORDERBOOK;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Test_Accumulator)
	{
	public:

		TEST_METHOD(Test_Accumulator_not_init_add)
		{
			try
			{
				accumulator acc;
				order_item order;
				acc.add_order(order);

				Assert::Fail(L"Exception missing.");
			}
			catch (const exception & e)
			{
				//
			}
		}

		TEST_METHOD(Test_Accumulator_not_init_remove)
		{
			try
			{
				accumulator acc;
				order_item order;
				acc.remove_order(1, 1);

				Assert::Fail(L"Exception missing.");
			}
			catch (const exception & e)
			{
				//
			}
		}

		TEST_METHOD(Test_Accumulator_init)
		{
			try
			{
				accumulator acc;
				{
					shared_ptr<order_book_iface> ptr(new order_book());
					acc.init(ptr);
				}
				order_item order;
				acc.add_order(order);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}
		}

		TEST_METHOD(Test_Accumulator_remove_from_empty)
		{
			accumulator acc;
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);
			}

			try
			{
				acc.remove_order(1, 1);
				Assert::Fail(L"Exception missing.");
			}
			catch (const exception & e)
			{
				//
			}
		}

		TEST_METHOD(Test_Accumulator_remove_invalid_order)
		{
			accumulator acc;
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);
			}

			try
			{
				order_item order(1, 1, 1);
				acc.add_order(order);
				acc.remove_order(2, 1);
				Assert::Fail(L"Exception missing.");
			}
			catch (const exception & e)
			{
				//
			}
		}

		TEST_METHOD(Test_Accumulator_default_average)
		{
			accumulator acc;
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);
			}

			Assert::IsTrue(acc.average_highest_price() == 0);
		}

		TEST_METHOD(Test_Accumulator_result1)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 10);
				acc.add_order(o1);
				acc.remove_order(o1.id(), 2000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 10);
		}

		TEST_METHOD(Test_Accumulator_result2)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 10);
				order_item o2(2, 2000, 20);

				acc.add_order(o1);
				acc.add_order(o2);

				acc.remove_order(o2.id(), 3000);
				acc.remove_order(o1.id(), 4000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 40000 / (double)3000);
		}

		TEST_METHOD(Test_Accumulator_result3)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 20);
				order_item o2(2, 2000, 10);

				acc.add_order(o1);
				acc.add_order(o2);

				acc.remove_order(o2.id(), 3000);
				acc.remove_order(o1.id(), 4000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 60000 / (double)3000);
		}

		TEST_METHOD(Test_Accumulator_result4)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 10);
				order_item o2(2, 2000, 20);

				acc.add_order(o1);
				acc.add_order(o2);

				acc.remove_order(o1.id(), 3000);
				acc.remove_order(o2.id(), 4000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 50000 / (double)3000);
		}

		TEST_METHOD(Test_Accumulator_result5)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 20);
				order_item o2(2, 2000, 10);

				acc.add_order(o1);
				acc.add_order(o2);

				acc.remove_order(o1.id(), 3000);
				acc.remove_order(o2.id(), 4000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 50000 / (double)3000);
		}

		TEST_METHOD(Test_Accumulator_result_gap)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 20);
				acc.add_order(o1);
				acc.remove_order(o1.id(), 2000);

				// 1000 msec gap.
				// "There may be periods when there are no orders (in this case, such periods should not be considered)."

				order_item o2(2, 3000, 10);
				acc.add_order(o2);
				acc.remove_order(o2.id(), 4000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 30000 / (double)2000);
		}

		TEST_METHOD(Test_Accumulator_result_gap2)
		{
			accumulator acc;
			try
			{
				shared_ptr<order_book_iface> ptr(new order_book());
				acc.init(ptr);

				order_item o1(1, 1000, 20);
				acc.add_order(o1);
				acc.remove_order(o1.id(), 2000);

				// 1000 msec gap.
				// "There may be periods when there are no orders (in this case, such periods should not be considered)."

				order_item o2(2, 3000, 10);
				acc.add_order(o2);
				acc.remove_order(o2.id(), 4000);

				// 1000 msec gap.

				order_item o3(2, 5000, 10);
				acc.add_order(o3);
				acc.remove_order(o3.id(), 6000);
			}
			catch (const exception & e)
			{
				string str = e.what();
				Assert::Fail(utils::widen(str).c_str());
			}

			Assert::IsTrue(acc.average_highest_price() == 40000 / (double)3000);
		}
	};
}