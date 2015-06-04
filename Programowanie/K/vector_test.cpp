#include "Vector.h"
#include <iostream>
#include <cassert>

using namespace std;

class A {
public:
   int a;
   int b;
   static int copyConstructorCount;
   static int moveConstructorCount;
   A(int a = 0, int b = 0) : a(a), b(b) {
//      cerr << "A " << *this << "\n";
   }

   A(A const& other) : a(other.a), b(other.b) {
      ++copyConstructorCount;
//      cerr << "A& " << *this << "\n";
   };

   A(A&& that) : a(that.a), b(that.b) {
      ++moveConstructorCount;
//      cerr << "A&& " << *this << "\n";
      that.a = that.b = 0;
   }

   ~A() {
//      cerr << "~A " << *this << "\n";
      a = b = 0;
   }

   A& operator=(A const&) = default;
   friend std::ostream& operator<<(std::ostream& out, const A& a) {
      out << "(" << a.a << ", " << a.b << ")";
      return out;
   }
};
int A::copyConstructorCount = 0;
int A::moveConstructorCount = 0;

template<typename T>
void printVector(const Vector<T>& v) {
   cerr << "size: " << v.Size()
         << " capacity: " << v.Capacity()
         << " min capacity: " << v.MinCapacity()
         << "\n";
   for (auto i = 0; i < v.Size(); ++i)
      cerr << v[i] << " ";
   cerr << "\n";
}

template<typename T>
void assertSizes(const Vector<T>& v, size_t size, size_t cap, size_t minCap) {
//   printVector(v);
   assert(v.Size() == size);
   assert(v.Capacity() >= cap);
   assert(v.MinCapacity() == minCap);
   size_t m = max(size, minCap);
   assert(m <= v.Capacity() && v.Capacity() <= 4 * m);
}

template<typename T, typename... Args>
void testVectorWithGivenType(Args&& ... args) {
   cout << "Constructors...\n";
   Vector<T> v1;
   assert(v1.Size() == 0);
   assert(v1.Capacity() == 0);
   assert(v1.MinCapacity() == 0);
   v1.Reserve(10);
   assertSizes(v1, 0, 10, 10);

   cout << "PushBack...\n";
   for(int i = 0; i < 20; ++i)
      v1.PushBack(T(std::forward<Args>(args)...));
   assertSizes(v1, 20, 20, 10);

   cout << "Assignment...\n";
   for(int i = 0; i < 19; ++i)
      v1[i] = v1[i+1];

   cout << "'Out of range' exception...\n";
   int x = -1;
   try {
      v1[20];
   }
   catch(std::out_of_range const& e) {
      x = 0;
   }
   assert(x == 0);

   cout << "PopBack...\n";
   for(int i = 0; i < 15; ++i)
      v1.PopBack();
   assertSizes(v1, 5, 10, 10);
   v1.PopBack();
   v1.PopBack();
   v1.PopBack();
   v1.Reserve(1);
   assertSizes(v1, 2, 2, 1);

   cout << "Copy constructor...\n";
   Vector<T> v2(v1);
   assertSizes(v2, 2, 2, 1); // not sure, if capacity and min capacity should be copied

   cout << "EmplaceBack...\n";
   for(int i = 0; i < 5; ++i)
      v2.EmplaceBack();
   assertSizes(v1, 2, 2, 1);
   assertSizes(v2, 7, 7, 1);

   cout << "Clear...\n";
   v1.Clear();
   assertSizes(v1, 0, 1, 1);
   assertSizes(v2, 7, 7, 1);

   cout << "Reserve and PushBack...\n";
   v2.Reserve(500);
   for(int i = 0; i < 100; ++i)
      v1.PushBack(T(std::forward<Args>(args)...));
   assertSizes(v1, 100, 100, 1);
   assertSizes(v2, 7, 500, 500);

   cout << "Move constructor...\n";
   Vector<T> v3(std::move(v1));
   assertSizes(v1, 0, 0, 0);
   assertSizes(v3, 100, 100, 1);
   for(int i = 0; i < 80; ++i)
      v3.PopBack();
   assertSizes(v3, 20, 20, 1);
   for(int i = 0; i < 5; ++i)
      v3.EmplaceBack(v2[i]);
   assertSizes(v2, 7, 500, 500);
   assertSizes(v3, 25, 25, 1);

   cout << "Reserve and Clear (test invariant)...\n";
   v3.Reserve(15);
   assertSizes(v3, 25, 25, 15);
   v3.Clear();
   assertSizes(v3, 0, 15, 15);

   cout << "Copy assignment operator...\n";
   Vector<T> v7;
   v7 = v2;
   v7.Reserve(100);
   assertSizes(v7, 7, 100, 100);
   for(int i = 0; i < 10; ++i)
      v7.PushBack(T(std::forward<Args>(args)...));
   assertSizes(v2, 7, 500, 500);
   assertSizes(v7, 17, 100, 100);
   v7.Reserve(10);
   assertSizes(v7, 17, 17, 10);
   v7.Clear();
   assertSizes(v2, 7, 500, 500);
   assertSizes(v7, 0, 10, 10);

   cout << "Assignment from locally created Vectors...\n";
   Vector<T> v8;
   {
      Vector<T> v4(10);
      Vector<T> v5(v2);
      assertSizes(v2, 7, 500, 500);
      assertSizes(v5, 7, 500, 500);
      cout << "Move constructor...\n";
      Vector<T> v6(std::move(v2));
      assertSizes(v2, 0, 0, 0);
      assertSizes(v6, 7, 500, 500);

      for(int i = 0; i < 10; ++i)
         v2.PushBack(T(std::forward<Args>(args)...));
      v5.Reserve(10);
      for(int i = 0; i < 20; ++i)
         v5.PushBack(T(std::forward<Args>(args)...));
      for(int i = 0; i < 30; ++i)
         v6.PushBack(T(std::forward<Args>(args)...));

      assertSizes(v2, 10, 10, 0);
      assertSizes(v5, 27, 27, 10);
      assertSizes(v6, 37, 500, 500);

      v7 = v5;
      assertSizes(v5, 27, 27, 10);
      assertSizes(v7, 27, 27, 10);

      cout << "Move assignment...\n";
      v8 = std::move(v6);
      assertSizes(v8, 37, 500, 500);
      assertSizes(v6, 0, 0, 0);
   }
   cout << "Check if size preserved after getting out of local scope...\n";
   assertSizes(v7, 27, 27, 10);
   assertSizes(v8, 37, 500, 500);
   assertSizes(v2, 10, 10, 0);

   cout << "Self assignment...\n";
   v8 = v8;
   assertSizes(v8, 37, 500, 500);
}

void testVectorAssignments() {
   Vector<int> v1(100);
   Vector<int> v2(50);
   assertSizes(v1, 0, 100, 100);
   assertSizes(v2, 0, 50, 50);
   cout << "v1 = v2\n";
   v1 = v2;
   assertSizes(v1, 0, 50, 50);
   assertSizes(v2, 0, 50, 50);
   v2.Reserve(30);
   assertSizes(v1, 0, 50, 50);
   assertSizes(v2, 0, 30, 30);
   cout << "v1 = move(v2)\n";
   v1 = std::move(v2);
   assertSizes(v1, 0, 30, 30);
   assertSizes(v2, 0, 0, 0);
   cout << "v2 = temp\n";
   v2 = Vector<int>(10);
   assertSizes(v2, 0, 10, 10);
   cout << "v2 = v2\n";
   v2 = v2;
   assertSizes(v2, 0, 10, 10);

   const size_t SIZE = 1e5;
   for(int i = 0; i < SIZE; ++i)
      v1.PushBack(i);
   v2 = v1;
   while(v2.Size() > 0)
      v2.PopBack();
   cout << "v2 empty, v1 full\n";
   assertSizes(v1, SIZE, SIZE, 30);
   assertSizes(v2, 0, 30, 30);

   cout << "chained assignment\n";
   Vector<int> v3;
   Vector<int> v4 = v3 = v2 = v1;

   assertSizes(v1, SIZE, SIZE, 30);
   assertSizes(v2, SIZE, SIZE, 30);
   assertSizes(v3, SIZE, SIZE, 30);
   assertSizes(v4, SIZE, SIZE, 30);
}

void testAddingRemoving() {
   Vector<A> va(20);
   cout << "PushBack\n";
   A::copyConstructorCount = 0;
   va.PushBack(A(1, 1));
   va.PushBack(std::move(A(2, 2)));
   assert(va[0].a == 1 && va[0].b == 1);
   assert(va[1].a == 2 && va[1].b == 2);
   assert(A::copyConstructorCount == 0);

   A a(3, 3);
   va.PushBack(a);
   va.PushBack(std::move(a));
   assert(va[2].a == 3 && va[2].b == 3);
   assert(va[3].a == 3 && va[3].b == 3);
   assert(A::copyConstructorCount == 1);
   assert(a.a == 0 && a.b == 0);

   cout << "EmplaceBack\n";
   A::copyConstructorCount = 0;
   A::moveConstructorCount = 0;
   va.EmplaceBack();
   va.EmplaceBack(-1, -1);
   assert(A::copyConstructorCount == 0);
   assert(A::moveConstructorCount == 0);

   assert(va[4].a == 0 && va[4].b == 0);
   assert(va[5].a == -1 && va[5].b == -1);

   cout << "PopBack on empty Vector should do nothing!\n";
   for(int i = 0; i < 100; ++i)
      va.PopBack();

   cout << "PushBack elements from itself\n";
   Vector<A> vb;
   vb.EmplaceBack(1, 1);
   for(int i = 0; i < 10000; ++i)
      vb.PushBack(vb[0]);
   for(int i = 0; i <= 10000; ++i)
      assert(vb[i].a == 1 && vb[i].b == 1);
}

int main() {
   /***************** README ********************
    * First of all - it should compile.
    * All tests are checked by function 'assert()', so when something's wrong,
    * the program will be just terminated 
    *********************************************/

   cout << "------ Test: Vector<char>\n";
   testVectorWithGivenType<char>('x');
   cout << "\n------ Test: Vector<int>\n";
   testVectorWithGivenType<int>(-1);
   cout << "\n------ Test: Vector<string>\n";
   testVectorWithGivenType<string>("DUPA");
   cout << "\n------ Test: Vector<Vector<string>>\n";
   testVectorWithGivenType<Vector<string>>();
   cout << "\n------ Test: Vector<Vector<Vector<Vector<string>>>>\n";
   testVectorWithGivenType<Vector<Vector<Vector<string>>>>();

   cout << "\n------ Test assignments\n";
   testVectorAssignments();

   cout << "\n------ Test adding and removing elements\n";
   testAddingRemoving();

   cout << "\n------ Test efficiency during reallocation -> use prefer move constructor to copy constructor\n";
   Vector<A> va;
   A::copyConstructorCount = 0;
   va.EmplaceBack(1, 1);
   assert(va.Capacity() <= 4);
   for (int i = 0; i < 4; ++i)
      va.EmplaceBack(1, 1);
   assert(A::copyConstructorCount == 0);
   cout << "\n################# All tests passed! #################\n";
   return 0;
}