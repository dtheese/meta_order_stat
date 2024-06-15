#include <iostream>

using namespace std;

struct nil;

template<int N, typename T>
struct value_list
{
   enum {RESULT = N};
};

#define CREATE_1(N1) value_list<(N1), nil>
#define CREATE_2(N1, N2) value_list<(N1), CREATE_1((N2)) >
#define CREATE_3(N1, N2, N3) value_list<(N1), CREATE_2((N2), (N3)) >
#define CREATE_4(N1, N2, N3, N4) value_list<(N1), CREATE_3((N2), (N3), (N4)) >

#define CREATE_5(N1, N2, N3, N4, N5) value_list<(N1),\
CREATE_4((N2), (N3), (N4), (N5)) >

#define CREATE_6(N1, N2, N3, N4, N5, N6) value_list<(N1),\
CREATE_5((N2), (N3), (N4), (N5), (N6)) >

#define CREATE_7(N1, N2, N3, N4, N5, N6, N7) value_list<(N1),\
CREATE_6((N2), (N3), (N4), (N5), (N6), (N7)) >

#define CREATE_8(N1, N2, N3, N4, N5, N6, N7, N8) value_list<(N1),\
CREATE_7((N2), (N3), (N4), (N5), (N6), (N7), (N8)) >

#define CREATE_9(N1, N2, N3, N4, N5, N6, N7, N8, N9) value_list<(N1),\
CREATE_8((N2), (N3), (N4), (N5), (N6), (N7), (N8), (N9)) >

#define CREATE_10(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10) value_list<(N1),\
CREATE_9((N2), (N3), (N4), (N5), (N6), (N7), (N8), (N9), (N10)) >

template<bool B, typename T1, typename T2>
struct type_selector
{
   typedef T1 RESULT;
};

template<typename T1, typename T2>
struct type_selector<false, T1, T2>
{
   typedef T2 RESULT;
};

template<typename T1, typename T2>
struct append;

template<>
struct append<nil, nil>
{
   typedef nil RESULT;
};

template<int N, typename T>
struct append<nil, value_list<N, T> >
{
   typedef value_list<N, T> RESULT;
};

template<int N, typename T>
struct append<value_list<N, T>, nil>
{
   typedef value_list<N, T> RESULT;
};

template<int N1, typename T1, int N2, typename T2>
struct append<value_list<N1, T1>, value_list<N2, T2> >
{
   typedef value_list<N1, typename append<T1, value_list<N2, T2> >::RESULT>
           RESULT;
};

template<typename T>
struct meta_size
{
   enum {RESULT = 0};
};

template<int N>
struct meta_size<value_list<N, nil> >
{
   enum {RESULT = 1};
};

template<int N, typename T>
struct meta_size<value_list<N, T> >
{
   enum {RESULT = 1 + meta_size<T>::RESULT};
};

template<typename T, int N>
struct all_less_or_equal;

template<int N>
struct all_less_or_equal<nil, N>
{
   typedef nil RESULT;
};

template<int N1, typename T, int N2>
struct all_less_or_equal<value_list<N1, T> , N2>
{
   typedef typename type_selector<
                                    (N1 <= N2),
                                    typename append<
                                       value_list<N1, nil>,
                                       typename all_less_or_equal<T, N2>::RESULT
                                                   >::RESULT,
                                    typename all_less_or_equal<T, N2>::RESULT
                                 >::RESULT RESULT;
};

template<typename T, int N>
struct all_greater;

template<int N>
struct all_greater<nil, N>
{
   typedef nil RESULT;
};

template<int N1, typename T, int N2>
struct all_greater<value_list<N1, T> , N2>
{
   typedef typename type_selector<
                                    (N1 > N2),
                                    typename append<
                                             value_list<N1, nil>,
                                             typename all_greater<T, N2>::RESULT
                                                   >::RESULT,
                                    typename all_greater<T, N2>::RESULT
                                 >::RESULT RESULT;
};

template<typename T>
struct meta_partition;

template<int N, typename T>
struct meta_partition<value_list<N, T> >
{
   typedef typename all_less_or_equal<T, N>::RESULT LEFT;
   typedef value_list<N, nil>                       MIDDLE;
   typedef typename all_greater<T, N>::RESULT       RIGHT;

   typedef typename append<
                             LEFT,
                             typename append<MIDDLE, RIGHT>::RESULT
                          >::RESULT
           RESULT;
};

template<typename T, int N>
struct order_stat;

template<int N1, typename T, int N2>
struct order_stat<value_list<N1, T> , N2>
{
   typedef typename meta_partition<value_list<N1, T> >::LEFT   LEFT;
   typedef typename meta_partition<value_list<N1, T> >::MIDDLE MIDDLE;
   typedef typename meta_partition<value_list<N1, T> >::RIGHT  RIGHT;

   enum
   {
      PARTITION_LOCATION =
           meta_size<LEFT>::RESULT + 1
   };

   typedef typename type_selector<
                                    (N2 == PARTITION_LOCATION),
                                    MIDDLE,
                                    typename type_selector<
                                                      (N2 < PARTITION_LOCATION),
                                                      order_stat<LEFT, N2>,
                                                      order_stat<
                                                         RIGHT,
                                                         N2 - PARTITION_LOCATION
                                                                >
                                                          >::RESULT
                                 >::RESULT TEMP_TYPE;

   enum
   {
      RESULT = TEMP_TYPE::RESULT
   };
};

int main()
{
   typedef CREATE_10(14, 11, 20, 19, 16, 15, 12, 13, 18, 17) the_list;

   cout << order_stat<the_list, 10>::RESULT << endl;
   cout << order_stat<the_list, 9>::RESULT << endl;
   cout << order_stat<the_list, 8>::RESULT << endl;
   cout << order_stat<the_list, 7>::RESULT << endl;
   cout << order_stat<the_list, 6>::RESULT << endl;
   cout << order_stat<the_list, 5>::RESULT << endl;
   cout << order_stat<the_list, 4>::RESULT << endl;
   cout << order_stat<the_list, 3>::RESULT << endl;
   cout << order_stat<the_list, 2>::RESULT << endl;
   cout << order_stat<the_list, 1>::RESULT << endl;
}
