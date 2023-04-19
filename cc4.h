/*----------------------------------------- CC: CONVENIENT CONTAINERS v1.0.3 -------------------------------------------

This library provides usability-oriented generic containers (vectors, linked lists, unordered maps, and unordered sets).

Features:

- Fully generic API (no need to specify element or key type except when first declaring a container).
- No need to pre-declare container types per element or key/element pair.
- Type safety.
- User-defined destructor, comparison, and hash functions associated with element and key types.
- Handles memory allocation failure.
- Single header.
- Compiles in C and C++.

Requires C23, or C11 and compiler support for __typeof__, or C++11.

Tested with GCC, MinGW, and Clang.

#including the library:

  Place this at the top of your file/s:

    #include "cc.h"

  The following can be #defined before #including the library in any file:

    #define CC4_NO_SHORT_NAMES
      By default, CC exposes API macros without the "cc4_" prefix.
      Define this flag to withhold the unprefixed names.

  The following can be #defined anywhere and affect all calls to API macros where the definition is visible:
  
    #define CC4_REALLOC our_realloc
      Causes API macros to use a custom realloc function rather than the one in the standard library.

    #define CC4_FREE our_free
      Causes API macros to use a custom free function rather than the one in the standard library.

API:

  General notes:

  - API macros may evaluate their first argument - the pointer to the container - multiple times, so never use
    expressions with side effects (e.g. &our_containers[ ++i ] ) for that argument. In GCC and Clang, attempting to do
    so will cause a compiler warning. All other arguments are only evaluated once.
  - If CC4_NO_SHORT_NAMES was declared, all API macros are prefixed with "cc4_".
  - Duplicating a container handle via assignment and then operating on the duplicate will invalidate the original.
    Hence, only create a duplicate via assignment (including through function parameters and return values) if you have
    finished with the original.
  - An iterator is a pointer to an element in the container or to the associated end (or r_end, if the container
    supports it). In the documentation below, these pointers are called "pointer-iterators".
  - In the documentation below, el_ty is the container's element type and key_ty is the container's key type (where
    applicable).

  All containers:

    The following macros behave the same way for all containers:

    void init( <any container type> *cntr )

      Initializes cntr for use.
      This call cannot fail (it does not allocate memory).

    bool init_clone( <any container type> *cntr, <same container type> *src )

      Initializes cntr as a shallow copy of src.
      Returns true, or false if unsuccessful due to memory allocation failure.

    size_t size( <any container type> *cntr )

      Returns the number of elements.

    void clear( <any container type> *cntr )

      Erases all elements, calling the element and key types' destructors if they exist.

    void cleanup( <any container type> *cntr )

      Erases all elements (calling the element and key types' destructors if they exist), frees any other memory
      associated with the container, and initializes the container for reuse.

    for_each( <any container type> *cntr, i_name )

      Creates a loop iterating over all elements from first to last.
      This macro declares a pointer-iterator (el_ty *) named i_name.
      It is equivalent to
        for( el_ty *i_name = first( cntr ); i_name != end( cntr ); i_name = next( cntr, i_name ) )
      and should be followed by the loop body.

  Vector (a dynamic array that stores elements in contiguous memory):

    vec( el_ty ) cntr

      Declares an uninitialized vector named cntr.

    size_t cap( vec( el_ty ) *cntr )

      Returns the current capacity.

    bool reserve( vec( el_ty ) *cntr, size_t n )

      Ensures that the the capacity is large enough to support n elements.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool resize( vec( el_ty ) *cntr, size_t n )

      Sets the number of elements to n.
      If n is above the current size, the new elements are uninitialized.
      If n is below the current size, the element type's destructor (if it exists) is called for each erased element.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( vec( el_ty ) *cntr )

      Shrinks the capacity to the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *get( vec( el_ty ) *cntr, size_t i )

      Returns an a pointer-iterator to the element at index i.

    el_ty *push( vec( el_ty ) *cntr, el_ty el )

      Inserts el at the end of the vector.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *push_n( vec( el_ty ) *cntr, el_ty *els, size_t n )

      Inserts n elements from array els at the end of the vector.
      Returns a pointer-iterator to the first new element, or NULL in the case of memory allocation failure.

    el_ty *insert( vec( el_ty ) *cntr, size_t i, el_ty el )

      Inserts el at index i.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *insert_n( vec( el_ty ) *cntr, size_t i, el_ty *els, size_t n )

      Inserts n elements from array els at index i.
      Returns a pointer-iterator to the first new element, or NULL in the case of memory allocation failure.

    el_ty *erase( vec( el_ty ) *cntr, size_t i )

      Erases the element at index i, calling the element type's destructor if it exists.
      Returns a pointer-iterator to the element after the erased element, or an end pointer-iterator if there
      is no subsequent element.

    el_ty *erase_n( vec( el_ty ) *cntr, size_t i, size_t n )

      Erases n elements beginning at index i, calling the element type's destructor, if it exists, for each
      erased element.
      Returns a pointer-iterator to the element after the erased elements, or an end pointer-iterator if there is no
      subsequent element.

    el_ty *end( vec( el_ty ) *cntr )

      Returns an end pointer-iterator.
      This call is synonymous with get( cntr, size( cntr ) ).

    el_ty *first( vec( el_ty ) *cntr )

      Returns an pointer-iterator to the first element, or an end pointer-iterator if the vector is empty.
      This call is synonymous with get( cntr, 0 ).

    el_ty *last( vec( el_ty ) *cntr )

      Returns a pointer-iterator to the last element.
      This call is synonymous with get( cntr, size( cntr ) - 1 ).
      It assumes that at the vector is not empty.

   el_ty *next( vec( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the element pointed to by i, or an end pointer-iterator if i
      points to the last element.

    Notes:
    - Vector pointer-iterators (including end) are invalidated by any API calls that cause memory reallocation.

  List (a doubly linked list with sentinels):

    list( el_ty ) cntr

      Declares an uninitialized list named cntr.

    el_ty *insert( list( el_ty ) *cntr, el_ty *i, el_ty el )

      Inserts el before pointer-iterator i.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.

    el_ty *push( list( el_ty ) *cntr, el_ty el )

      Inserts el at the end of the list.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      This call is synonymous with insert( cntr, end( cntr ), el ).

    el_ty *erase( list( el_ty ) *cntr, el_ty *i )

      Erases element pointed to by pointer-iterator i, calling the element type's destructor if it exists.
      Returns a pointer-iterator to the element after i, or an end pointer-iterator if i was the last element.

    bool splice( list( el_ty ) *cntr, el_ty *i, list( el_ty ) src, el_ty *src_i )

      Removes element pointed to by pointer-iterator src_i from src and inserts it before the element pointed to by
      pointer-iterator i in cntr.
      Returns true, or false if unsuccessful.
      This call only allocates memory, and therefore can only fail, if the list has not had any element inserted,
      pushed, or spliced into it since it was initialized.

    el_ty *first( list( el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the list is empty.

    el_ty *last( list( el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the list is empty.

    el_ty *r_end( list( el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the list. r_end acts as a sentinel node.

    el_ty *end( list( el_ty ) *cntr )

      Returns an end pointer-iterator for the list. End acts as a sentinel node.

    el_ty *next( list( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the return value is an end pointer-iterator.
      If i points to r_end, the return value is a pointer-iterator to the first element, or an end pointer-iterator if
      the list is empty.

    el_ty *prev( list( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the return value is an r_end pointer-iterator.
      If i points to end, then the return value is a pointer-iterator to the last element, or an r_end pointer-iterator
      if the list is empty.

    r_for_each( list( el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    Notes:
    - List pointer-iterators (including r_end and end) are not invalidated by any API calls besides init and cleanup,
      unless they point to erased elements.

  Map (an unordered container associating elements with keys, implemented as a Robin Hood hash table):

    map( key_ty, el_ty ) cntr

      Declares an uninitialized map named cntr.
      key_ty must be a type, or alias for a type, for which comparison and hash functions have been defined.
      This requirement is enforced internally such that neglecting it causes a compiler error.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( map( key_ty, el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a map can support without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( map( key_ty, el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to support n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( map( key_ty, el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el with the specified key.
      If an element with the same key already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      If adding one element would violate the map's max load factor, failure can occur even if it already contains the
      key.

    el_ty *get( map( key_ty, el_ty ) *cntr, key_ty key )

      Returns a pointer-iterator to the element with the specified key, or NULL if no such element exists.

    el_ty *get_or_insert( map( key_ty, el_ty ) *cntr, key_ty key, el_ty el )

      Inserts element el if no element with the specified key already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing
      element with the same key, or NULL in the case of memory allocation failure.
      If adding one element would violate the map's max load factor, failure can occur even if it already contains the
      key.
      Determine whether an element was inserted by comparing the map's size before and after the call.

    const key_ty *key_for( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a const pointer to the key for the element pointed to by pointer-iterator i.

    bool erase( map( key_ty, el_ty ) *cntr, key_ty key )

      Erases the element with the specified key, if it exists.
      Returns true if an element was erased, or false if no such element exists.

    void erase_itr( map( key_ty, el_ty ) *cntr, el_ty *i )

      Erases the element pointed to by pointer-iterator i.

    el_ty *first( map( key_ty, el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the map is empty.

    el_ty *last( map( key_ty, el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the map is empty.

    el_ty *r_end( map( key_ty, el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the map.

    el_ty *end( map( key_ty, el_ty ) *cntr )

      Returns an end pointer-iterator for the map.

    el_ty *next( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the value returned is an end pointer-iterator.
      If i points to r_end, the value returned points to the first element, or is an end pointer-iterator if the map is
      empty.

    el_ty *prev( map( key_ty, el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the value returned is an r_end pointer-iterator.
      If i points to end, then the value returned points to the last element, or is an r_end pointer-iterator if the map
      is empty.

    for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )

      Creates a loop iterating over all elements from first to last, with easy access to the corresponding keys.
      This macro declares a pointer to the key (const key_ty *) named key_ptr_name and a pointer-iterator (el_ty *)
      named i_name.
      It should be followed by the body of the loop.

    r_for_each( map( key_ty, el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    r_for_each( map( key_ty, el_ty ) *cntr, key_ptr_name, i_name )

      Creates a loop iterating over all elements from last to first, with easy access to the corresponding keys.
      This macro declares a pointer to the key (const key_ty *) named key_ptr_name and a pointer-iterator (el_ty *)
      named i_name.
      It should be followed by the body of the loop.

    Notes:
    - Map pointer-iterators (including r_end and end) may be invalidated by any API calls that cause memory
      reallocation.

  Set (Robin Hood hash table for elements without a separate key):

    set( el_ty ) cntr

      Declares an uninitialized set named cntr.
      el_ty must be a type, or alias for a type, for which comparison and hash functions have been defined.
      This requirement is enforced internally such that neglecting it causes a compiler error.
      For types with in-built comparison and hash functions, and for details on how to declare new comparison and hash
      functions, see "Destructor, comparison, and hash functions and custom max load factors" below.

    size_t cap( set( el_ty ) *cntr )

      Returns the current capacity, i.e. bucket count.
      Note that the number of elements a set can support without rehashing is not its capacity but its capacity
      multiplied by the max load factor associated with its key type.

    bool reserve( set( el_ty ) *cntr, size_t n )

      Ensures that the capacity is large enough to support n elements without rehashing.
      Returns true, or false if unsuccessful due to memory allocation failure.

    bool shrink( set( el_ty ) *cntr )

      Shrinks the capacity to best accommodate the current size.
      Returns true, or false if unsuccessful due to memory allocation failure.

    el_ty *insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el.
      If the element already exists, the existing element is replaced.
      Returns a pointer-iterator to the new element, or NULL in the case of memory allocation failure.
      Note that if adding one element would violate the set's max load factor, failure can occur even if it already
      contains el.

    el_ty *get( set( el_ty ) *cntr, el_ty el )

      Returns a pointer-iterator to element el, or NULL if no such element exists.

    el_ty *get_or_insert( set( el_ty ) *cntr, el_ty el )

      Inserts element el if it does not already exist.
      Returns a pointer-iterator to the new element if it was inserted, or a pointer-iterator to the existing element,
      or NULL in the case of memory allocation failure.
      If adding one element would violate the set's max load factor, failure can occur even if it already contains the
      element.
      Determine whether an element was inserted by comparing the set's size before and after the call.

    bool erase( set( el_ty ) *cntr, el_ty el )

      Erases the element el, if it exists.
      Returns true if an element was erased, or false if no such element exists.

    el_ty *first( set( el_ty ) *cntr )

      Returns a pointer-iterator to the first element, or an end pointer-iterator if the set is empty.

    el_ty *last( set( el_ty ) *cntr )

      Returns a pointer-iterator to the last element, or an r_end pointer-iterator if the set is empty.

    el_ty *r_end( set( el_ty ) *cntr )

      Returns an r_end (reverse end) pointer-iterator for the set.

    el_ty *end( set( el_ty ) *cntr )

      Returns an end pointer-iterator for the set.

    el_ty *next( set( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element after the one pointed to by i.
      If i points to the last element, the pointer-iterator returned is an end pointer-iterator.
      If i points to r_end, then the pointer-iterator returned points to the first element, or is an end
      pointer-iterator if the set is empty.

    el_ty *prev( set( el_ty ) *cntr, el_ty *i )

      Returns a pointer-iterator to the element before the one pointed to by i.
      If i points to the first element, the return value is an r_end pointer-iterator.
      If i points to end, then the pointer-iterator returned points to the last element, or is an r_end pointer-iterator
      if the set is empty.

    r_for_each( set( el_ty ) *cntr, i_name )

      Creates a loop iterating over all elements from last to first.
      This macro declares an el_ty * pointer-iterator named i_name.
      It is equivalent to
        for( el_ty *i_name = last( cntr ); i_name != r_end( cntr ); i_name = prev( cntr, i_name ) )
      and should be followed by the body of the loop.

    Notes:
    - Set pointer-iterators (including r_end and end) may be invalidated by any API calls that cause memory
      reallocation.

  Destructor, comparison, and hash functions and custom max load factors:

    This part of the API allows the user to define custom destructor, comparison, and hash functions and max load
    factors for a type.
    Once these functions are defined, any container using that type for its elements or keys will call them
    automatically.
    Once the max load factor is defined, any map using the type for its key and any set using the type for its elements
    will use the defined load factor to determine when rehashing is necessary.

    #define CC4_DTOR ty, { function body }
    #include "cc.h"

      Defines a destructor for type ty.
      The signature of the function is void ( ty val ).

    #define CC4_CMPR ty, { function body }
    #include "cc.h"

      Defines a comparison function for type ty.
      The signature of the function is int ( ty val_1, ty val_2 ).
      The function should return 0 if val_1 and val_2 are equal, a negative integer if val_1 is less than val_2, and a
      positive integer if val_1 is more than val_2.

    #define CC4_HASH ty, { function body }
    #include "cc.h"

      Defines a hash function for type ty.
      The signature of the function is size_t ( ty val ).
      The function should return the hash of val.

    #define CC4_LOAD ty, max_load_factor

      Defines the max load factor for type ty.
      max_load_factor should be a float or double between 0.0 and 1.0.
      The default max load factor is 0.8.
    
    Trivial example:

      typedef struct { int x; } our_type;
      #define CC4_DTOR our_type, { printf( "!%d\n", val.x ); }
      #define CC4_CMPR our_type, { return ( val_1.x > val_2.x ) - ( val_1.x < val_2.x ); }
      #define CC4_HASH our_type, { return val.x * 2654435761ull; }
      #define CC4_LOAD our_type, 0.5
      #include "cc.h"

    Notes:
    - These functions are inline and have static scope, so you need to either redefine them in each translation unit
      from which they should be called or (preferably) define them in a shared header. For structs or unions, a sensible
      place to define them would be immediately after the definition of the struct or union.
    - Only one destructor, comparison, or hash function or max load factor should be defined by the user for each type.
    - #including cc.h in these cases does not #include the full header, so you still need to #include it separately
      at the top of your files.
    - In-built comparison and hash functions are already defined for the following types: char, unsigned char, signed
      char, unsigned short, short, unsigned int, int, unsigned long, long, unsigned long long, long long, size_t, and
      char * (a NULL-terminated string). Defining a comparsion or hash function for one of these types will overwrite
      the in-built function.

Version history:

  XXXXXXXXXX 1.0.3: Completed refractor that reduces compile speed by approximate XX% in C with GCC (though C++ compile
                    speed has increased).
                    This was achieved by reducing the number of _Generic expressions that API calls use (by extracting
                    multiple key-related data from a single expression) and by reducing the work done inside _Generic
                    expressions.
                    Also introduced performance improvements into maps and sets and corrected a bug that could cause
                    map and set probe length offset integers to be unaligned.
  04/02/2023 1.0.2: Fixed bug preventing custom hash table load factors from taking effect when CC4_LOAD is defined in
                    isolation of CC4_HASH, CC4_CMPR, or CC4_DTOR.
                    Made minor adjustment to code comments and documentation so that they are more consistent.
  27/01/2023 1.0.1: Made minor corrections to code comments.
  26/12/2022 1.0.0: Initial release.

License (MIT):

  Copyright (c) 2022-2023 Jackson L. Allan

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#if !defined( CC4_DTOR ) && !defined( CC4_CMPR ) && !defined( CC4_HASH ) && !defined( CC4_LOAD )/*------------------------*/
/*                                                                                                                    */
/*                                                REGULAR HEADER MODE                                                 */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef CC4_NO_SHORT_NAMES
#define vec( ... )           cc4_vec( __VA_ARGS__ )
#define list( ... )          cc4_list( __VA_ARGS__ )
#define map( ... )           cc4_map( __VA_ARGS__ )
#define set( ... )           cc4_set( __VA_ARGS__ )
#define init( ... )          cc4_init( __VA_ARGS__ )
#define init_clone( ... )    cc4_init_clone( __VA_ARGS__ )
#define size( ... )          cc4_size( __VA_ARGS__ )
#define cap( ... )           cc4_cap( __VA_ARGS__ )
#define reserve( ... )       cc4_reserve( __VA_ARGS__ )
#define resize( ... )        cc4_resize( __VA_ARGS__ )
#define shrink( ... )        cc4_shrink( __VA_ARGS__ )
#define insert( ... )        cc4_insert( __VA_ARGS__ )
#define insert_n( ... )      cc4_insert_n( __VA_ARGS__ )
#define get_or_insert( ... ) cc4_get_or_insert( __VA_ARGS__ )
#define push( ... )          cc4_push( __VA_ARGS__ )
#define push_n( ... )        cc4_push_n( __VA_ARGS__ )
#define splice( ... )        cc4_splice( __VA_ARGS__ )
#define get( ... )           cc4_get( __VA_ARGS__ )
#define key_for( ... )       cc4_key_for( __VA_ARGS__ )
#define erase( ... )         cc4_erase( __VA_ARGS__ )
#define erase_n( ... )       cc4_erase_n( __VA_ARGS__ )
#define erase_itr( ... )     cc4_erase_itr( __VA_ARGS__ )
#define clear( ... )         cc4_clear( __VA_ARGS__ ) 
#define cleanup( ... )       cc4_cleanup( __VA_ARGS__ )
#define first( ... )         cc4_first( __VA_ARGS__ )
#define last( ... )          cc4_last( __VA_ARGS__ )
#define r_end( ... )         cc4_r_end( __VA_ARGS__ )
#define end( ... )           cc4_end( __VA_ARGS__ )
#define next( ... )          cc4_next( __VA_ARGS__ )
#define prev( ... )          cc4_prev( __VA_ARGS__ )
#define for_each( ... )      cc4_for_each( __VA_ARGS__ )
#define r_for_each( ... )    cc4_r_for_each( __VA_ARGS__ )
#endif

#ifndef CC4_H
#define CC4_H

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                    Preliminary                                                     */
/*--------------------------------------------------------------------------------------------------------------------*/

// _Static_assert alternative that can be used inside an expression.
#define CC4_STATIC_ASSERT( xp ) (void)sizeof( char[ (xp) ? 1 : -1 ] )

// CC4_MAKE_LVAL_COPY macro for making an addressable temporary copy of a variable or expression.
// The copy is valid until at least the end of full expression surrounding the macro call.
// In C, this is accomplished using a compound literal.
// In C++, we use rvalue reference magic.
// This is used to pass a pointers to elements and keys (which the user may have provided as rvalues) into container
// functions.
#ifdef __cplusplus
template<class ty> ty& cc4_unmove( ty&& var ) { return var; }
#define CC4_MAKE_LVAL_COPY( ty, xp ) cc4_unmove( (ty)( xp ) )
#else
#define CC4_MAKE_LVAL_COPY( ty, xp ) *( ty[ 1 ] ){ xp }
#endif

// Macro used primarily for silencing unused-expression warnings for macros that return cast pointers.
// This issued seems to affect Clang in particular.
// GCC, on the other hand, seems to accept that pointer casts may be redundant.
#ifdef __cplusplus
template<typename ty_1, typename ty_2> ty_1 cc4_maybe_unused( ty_2 xp ){ return (ty_1)xp; }
#define CC4_CAST_MAYBE_UNUSED( ty, xp ) cc4_maybe_unused<ty>( xp )
#else
#define CC4_CAST_MAYBE_UNUSED( ty, xp ) ( ( ty ){ 0 } = ( (ty)( xp ) ) )
#endif

// CC4_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2 is the same as above, except that it selects the type to which to cast based on
// a condition.
// This is neccessary because some API macros (e.g. cc4_erase) return either a pointer-iterator or a bool depending on
// the container type.

#ifdef __cplusplus

template<bool cond, class ty_1, class ty_2, class xp_ty, typename std::enable_if<cond, bool>::type = true> \
ty_1 cc4_if_then_cast_ty_1_else_cast_ty_2( xp_ty xp ){ return (ty_1)xp; }                                   \

template<bool cond, class ty_1, class ty_2, class xp_ty, typename std::enable_if<!cond, bool>::type = true> \
ty_2 cc4_if_then_cast_ty_1_else_cast_ty_2( xp_ty xp ){ return (ty_2)xp; }                                    \

#define CC4_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2( cond, ty_1, ty_2, xp ) \
cc4_if_then_cast_ty_1_else_cast_ty_2<cond, ty_1, ty_2>( xp )         \

#else

#define CC4_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2( cond, ty_1, ty_2, xp ) \
CC4_CAST_MAYBE_UNUSED(                                               \
  CC4_TYPEOF_XP(                                                     \
    _Generic( (char (*)[ 1 + (bool)( cond ) ]){ 0 },                \
        char (*)[ 1 ]: (ty_2){ 0 },                                 \
        char (*)[ 2 ]: (ty_1){ 0 }                                  \
    )                                                               \
  ),                                                                \
  ( xp )                                                            \
)                                                                   \

#endif

// In GCC and Clang, we can generate a warning if the user passes an expression that may have side effects as the first
// argument of API macros.
// If the expression could have side effects, the compiler will not able able to resolve a comparision of it with itself
// at compile time, which we can check using __builtin_constant_p.
// The warning itself is generated via a division by zero.
// This macro may produce false positives (e.g. for &our_containers[ our_func() ] where our_func is a pure function that
// always returns the same value), but that is a reasonable price to pay for more macro safety.

#ifdef __GNUC__

#define CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr )                                                            \
(void)(                                                                                                   \
  "WARNING: CONTAINER ARGUMENT MAY HAVE DUPLICATED SIDE EFFECTS" &&1/__builtin_constant_p((cntr)==(cntr)) \
)                                                                                                         \

#else
#define CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ) (void)0
#endif

// Typeof for expressions and abstract declarations.
#ifdef __cplusplus
#define CC4_TYPEOF_XP( xp ) std::decay<std::remove_reference<decltype( xp )>::type>::type
#define CC4_TYPEOF_TY( ty ) std::decay<std::remove_reference<decltype( std::declval<ty>() )>::type>::type
#else
// TODO: Add C23 check once C23 is supported by major compilers.
#define CC4_TYPEOF_XP( xp ) __typeof__( xp )
#define CC4_TYPEOF_TY( ty ) __typeof__( ty )
#endif

// CC4_SELECT_ON_NUM_ARGS macro for overloading API macros based on number of arguments.
#define CC4_CAT_2_( a, b ) a##b
#define CC4_CAT_2( a, b ) CC4_CAT_2_( a, b )
#define CC4_N_ARGS_( _1, _2, _3, _4, _5, _6, n, ... ) n
#define CC4_N_ARGS( ... ) CC4_N_ARGS_( __VA_ARGS__, _6, _5, _4, _3, _2, _1, x )
#define CC4_SELECT_ON_NUM_ARGS( func, ... ) CC4_CAT_2( func, CC4_N_ARGS( __VA_ARGS__ ) )( __VA_ARGS__ )

// If the user has defined CC4_REALLOC and CC4_FREE, then CC4_GET_REALLOC and CC4_GET_FREE are replaced with those macros.
// Otherwise, they are replaced by realloc and free from the standard library.
#define CC4_ARG_2_( _1, _2, ... ) _2
#define CC4_ARG_2( ... ) CC4_ARG_2_( __VA_ARGS__ )
#define CC4_REALLOC_COMMA ,
#define CC4_REALLOC_FN CC4_ARG_2( CC4_CAT_2( CC4_REALLOC, _COMMA ) realloc, CC4_REALLOC, )
#define CC4_FREE_COMMA ,
#define CC4_FREE_FN CC4_ARG_2( CC4_CAT_2( CC4_FREE, _COMMA ) free, CC4_FREE, )

// Macro used with CC4_STATIC_ASSERT to provide type safety in cc4_init_clone and cc4_splice calls.
#ifdef __cplusplus
#define CC4_IS_SAME_TY( a, b ) std::is_same<CC4_TYPEOF_XP( a ), CC4_TYPEOF_XP( b )>::value
#else
#define CC4_IS_SAME_TY( a, b ) _Generic( (a), CC4_TYPEOF_XP( b ): true, default: false )
#endif

// Macro for handling unused paramaters in most container functions that plug directly into API macros.
// These functions must provide a standardized signature across containers.
// The compiler should optimize away unused paramaters anyway, but we can nevertheless mark them as redundant.
#ifdef __cplusplus
#define CC4_UNUSED( ty, name ) ty
#else
#ifdef __GNUC__
#define CC4_UNUSED( ty, name ) __attribute__((unused)) ty name
#else
#define CC4_UNUSED( ty, name ) ty name
#endif
#endif

// Some functions that must return true/false must return the value in the form of a pointer.
// This is because they are paired in ternary expressions inside API macros with other functions for other containers
// that return a pointer (primarily cc4_erase).
// While any suitably aligned pointer - e.g. the container handle - would do, we declare a global cc4_dummy_true_ptr for
// the sake of code readability.
static max_align_t cc4_dummy_true;
static void *cc4_dummy_true_ptr = &cc4_dummy_true;

// Default max load factor for maps and sets.
#define CC4_DEFAULT_LOAD 0.75

// Types for comparison, hash, destructor, realloc, and free functions.
// These are only for internal use as user-provided comparison, hash, and destructor have a different signature (see
// documentation above).
typedef int ( *cc4_cmpr_fnptr_ty )( void *, void * );
typedef size_t ( *cc4_hash_fnptr_ty )( void * );
typedef void ( *cc4_dtor_fnptr_ty )( void * );
typedef void *( *cc4_realloc_fnptr_ty )( void *, size_t );
typedef void ( *cc4_free_fnptr_ty )( void * );

// Swaps a block of memory (used for Robin-Hooding in maps and sets).
// Implemented as a macro to ensure inlining.
#define CC4_MEMSWAP( a, b, size )                  \
{                                                 \
  char *aa = (char *)(a);                         \
  char *bb = (char *)(b);                         \
  size_t bytes_left = (size);                     \
                                                  \
  alignas( max_align_t ) char copy_buffer[ 256 ]; \
                                                  \
  while( bytes_left > 256 )                       \
  {                                               \
    memcpy( copy_buffer, aa, 256 );               \
    memcpy( aa, bb, 256 );                        \
    memcpy( bb, copy_buffer, 256 );               \
    aa += 256;                                    \
    bb += 256;                                    \
    bytes_left -= 256;                            \
  }                                               \
                                                  \
  memcpy( copy_buffer, aa, bytes_left );          \
  memcpy( aa, bb, bytes_left );                   \
  memcpy( bb, copy_buffer, bytes_left );          \
}                                                 \
/*
static inline __attribute__((always_inline)) void cc4_memswap( void *a, void *b, size_t size )
{
  alignas( max_align_t ) char copy_buffer[ 256 ];

  char *aa = (char *)a;
  char *bb = (char *)b;
  while( size > 256 )
  {
    memcpy( copy_buffer, aa, 256 );
    memcpy( aa, bb, 256 );
    memcpy( bb, copy_buffer, 256 );
    aa += 256;
    bb += 256;
    size -= 256;
  }

  memcpy( copy_buffer, aa, size );
  memcpy( aa, bb, size );
  memcpy( bb, copy_buffer, size );
}

static inline __attribute__((always_inline)) void cc4_memswap2( void *a, void *b, size_t size_ab, void *c, void *d, size_t size_cd )
{
  alignas( max_align_t ) char copy_buffer[ 256 ];

  char *aa = (char *)a;
  char *bb = (char *)b;
  while( size_ab > 256 )
  {
    memcpy( copy_buffer, aa, 256 );
    memcpy( aa, bb, 256 );
    memcpy( bb, copy_buffer, 256 );
    aa += 256;
    bb += 256;
    size_ab -= 256;
  }

  memcpy( copy_buffer, aa, size_ab );
  memcpy( aa, bb, size_ab );
  memcpy( bb, copy_buffer, size_ab );

  char *cc = (char *)c;
  char *dd = (char *)d;
  while( size_cd > 256 )
  {
    memcpy( copy_buffer, cc, 256 );
    memcpy( cc, dd, 256 );
    memcpy( dd, copy_buffer, 256 );
    cc += 256;
    bb += 256;
    size_cd -= 256;
  }

  memcpy( copy_buffer, cc, size_cd );
  memcpy( cc, dd, size_cd );
  memcpy( dd, copy_buffer, size_cd );
}*/

// Container ids to identify container type at compile-time.
#define CC4_VEC  1
#define CC4_LIST 2
#define CC4_MAP  3
#define CC4_SET  4

// Produces underlying function pointer type for a given element/key type pair.
#define CC4_MAKE_BASE_FNPTR_TY( el_ty, key_ty ) CC4_TYPEOF_TY( CC4_TYPEOF_TY( el_ty ) (*)( CC4_TYPEOF_TY( key_ty )* ) )

// Produces container handle for a given element type, key type, and container id.
// In other words, this macro creates a pointer that carries all the information needed to identify and operate on a
// container at compile time: el_ty (*(*)[ cntr_id ])( key_ty * ).
// That is a pointer to an array of function pointers whose signature is el_ty ( key_ty * ).
#define CC4_MAKE_CNTR_TY( el_ty, key_ty, cntr_id ) CC4_TYPEOF_TY( CC4_MAKE_BASE_FNPTR_TY( el_ty, key_ty )(*)[ cntr_id ] )

// API macros for declaring containers.
// The key type is the type uses to lookup elements, even in nonassociative containers (e.g. a vector's key type is a
// size_t array index).
// This simplifies API macros by eliminating the need to treat non-associative containers as special cases.

#define cc4_vec( el_ty )         CC4_MAKE_CNTR_TY( el_ty, size_t, CC4_VEC )

#define cc4_list( el_ty )        CC4_MAKE_CNTR_TY( el_ty, void *, CC4_LIST ) // List key is a pointer-iterator.

#define cc4_map( key_ty, el_ty ) CC4_MAKE_CNTR_TY(                                                       \
                                  el_ty,                                                               \
                                  key_ty,                                                              \
                                  CC4_MAP * ( (                                                         \
                                    /* Compiler error if key type lacks compare and hash functions. */ \
                                    CC4_HAS_CMPR( key_ty ) && CC4_HAS_HASH( key_ty ) &&                  \
                                    /* Compiler error if bucket layout constraints are violated. */    \
                                    CC4_SATISFIES_LAYOUT_CONSTRAINTS( key_ty, el_ty )                   \
                                  ) ? 1 : -1 )                                                         \
                                )                                                                      \

#define cc4_set( el_ty )         CC4_MAKE_CNTR_TY(                                                                   \
                                  /* As set simply wraps map, we use el_ty as both the element and key types. */   \
                                  /* This allows minimal changes to map macros and functions to make sets work. */ \
                                  el_ty,                                                                           \
                                  el_ty,                                                                           \
                                  CC4_SET * ( (                                                                     \
                                    /* Compiler error if key type lacks compare and hash functions. */             \
                                    CC4_HAS_CMPR( el_ty ) && CC4_HAS_HASH( el_ty ) &&                                \
                                    /* Compiler error if bucket layout constraints are violated. */                \
                                    CC4_SATISFIES_LAYOUT_CONSTRAINTS( el_ty, el_ty )                                \
                                  ) ? 1 : -1 )                                                                     \
                                )                                                                                  \

// Retrieves a container's id (CC4_VEC, CC4_LIST, etc.) from its handle.
#define CC4_CNTR_ID( cntr ) ( sizeof( *cntr ) / sizeof( **cntr ) )

// Retrieves a container's element type from its handle.
#define CC4_EL_TY( cntr ) CC4_TYPEOF_XP( (**cntr)( NULL ) )

// Easy access to a container's element and key sizes.
#define CC4_EL_SIZE( cntr )  sizeof( CC4_EL_TY( cntr ) )

// CC4_KEY_TY macros for retrieving a container's key type from its handle (i.e. from the argument of the base function
// pointer).
#ifdef __cplusplus // For C++, this can be done easily with the help of a template function.

template<typename el_ty, typename key_ty>
key_ty cc4_key_ty( el_ty (*)( key_ty * ) )
{
  key_ty dummy;
  return dummy;
}

#define CC4_KEY_TY( cntr ) CC4_TYPEOF_XP( cc4_key_ty( **cntr ) )

#else // For C, we need to use _Generic trickery to match the base function pointer type with a key type previously
      // coupled with a compare function.

#define CC4_KEY_TY_SLOT( n, arg ) CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( arg ), cc4_cmpr_##n##_ty ): ( cc4_cmpr_##n##_ty ){ 0 },
#define CC4_KEY_TY( cntr )                                                                         \
CC4_TYPEOF_XP(                                                                                     \
  _Generic( (**cntr),                                                                             \
    CC4_FOR_EACH_CMPR( CC4_KEY_TY_SLOT, cntr )                                                      \
    default: _Generic( (**cntr),                                                                  \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char ):               ( char ){ 0 },               \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char ):      ( unsigned char ){ 0 },      \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char ):        ( signed char ){ 0 },        \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short ):     ( unsigned short ){ 0 },     \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short ):              ( short ){ 0 },              \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int ):       ( unsigned int ){ 0 },       \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int ):                ( int ){ 0 },                \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long ):      ( unsigned long ){ 0 },      \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long ):               ( long ){ 0 },               \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long ): ( unsigned long long ){ 0 }, \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long ):          ( long long ){ 0 },          \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), cc4_maybe_size_t ):    ( size_t ){ 0 },             \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * ):             ( char * ){ 0 },             \
      CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), void * ):             ( void * ){ 0 },             \
      default: (char){ 0 } /* Nothing */                                                          \
    )                                                                                             \
  )                                                                                               \
)                                                                                                 \

#endif

// Probe length type for maps and sets (Robin Hood hash tables).
// An unsigned char would probably be fine, but we use unsigned int just in case.
// A probe length of 0 denotes an empty bucket, whereas a probe length of 1 denotes an element in its home bucket.
// This optimization allows us to eliminate separate checks for empty buckets.
typedef unsigned int cc4_probelen_ty;

// The functions associated with some containers require extra information about how elements and/or keys and other data
// are laid out in memory.
// In particular, maps and sets need information about their bucket layouts, which depend on their element and/or key
// types (a future implementation of ordered maps and sets will also require such data).
// This data is formed by extracting the key size and alignment and passing it, along with the element size and
// alignment and the container type id, into the cc4_layout function, which returns a uint64_t describing the layout.
// The key size and alignment are inferred via a _Generic macro that looks up the key type based on the default
// or user-supplied comparison functions, since all containers that require layout information also require a comparison
// function.
// Although this mechanism is convoluted, it has proven to compile much faster than other approaches (e.g. calculating
// the layout inside a _Generic macro).
// The compiler should optimize the entire mechanism into compile-time constants.

// The layout for a map bucket is:
//   +------------+----+------------+----+------------+----+
//   |     #1     | #2 |     #3     | #4 |     #5     | #6 |
//   +------------+----+------------+----+------------+----+
//   #1 Element.
//   #2 Element padding to key_ty alignment.
//   #3 Key.
//   #4 Key padding to cc4_probelen_ty alignment.
//   #5 Probe length.
//   #6 #5 Probe length padding to largerst of el_ty, key_ty, and cc4_probelen_ty alignments.

// The layout for a set bucket is:
//   +------------+----+------------+----+
//   |     #1     | #2 |     #3     | #4 |
//   +------------+----+------------+----+
//   #1 Element (el_ty and key_ty are the same).
//   #2 Padding to cc4_probelen_ty alignment.
//   #3 Probe length.
//   #4 Padding to el_ty alignment.

// The layout data passed into a container function is a uint64_t composed of a uint32_t denoting the key size, a
// uint8_t denoting the padding after the element, a uint8_t denoting the padding after the key, and a uint8_t
// denoting the padding after the probe length.
// The reason that a uint64_t, rather than a struct, is used is that GCC has trouble properly optimizing the passing
// of the struct - even if only 8 bytes - into some container functions (specifically cc4_map_insert), apparently because
// it declines to pass the struct by register.

// Macro for ensuring valid layout on container declaration.
// Since the key size is four bytes and the padding values each one bytes, the key size must be <= UINT32_MAX (about
// 4.3GB) and the alignment of the element and key must be <= UINT8_MAX + 1 (i.e. 256).
// It unlikely that these constraints could be violated, but we can check anyway.
#define CC4_SATISFIES_LAYOUT_CONSTRAINTS( key_ty, el_ty )                                                      \
( sizeof( key_ty ) <= UINT32_MAX && alignof( el_ty ) <= UINT8_MAX + 1 && alignof( key_ty ) <= UINT8_MAX + 1 ) \

// Macros and functions for constructing a layout.

#define CC4_PADDING( size, align ) ( ( ~(size) + 1 ) & ( (align) - 1 ) )
#define CC4_MAX( a, b ) ( (a) > (b) ? (a) : (b) )

#define CC4_MAP_EL_PADDING( el_size, key_align ) \
CC4_PADDING( el_size, key_align )                \

#define CC4_MAP_KEY_PADDING( el_size, key_size, key_align )                                            \
CC4_PADDING( el_size + CC4_MAP_EL_PADDING( el_size, key_align ) + key_size, alignof( cc4_probelen_ty ) ) \

#define CC4_MAP_PROBELEN_PADDING( el_size, el_align, key_size, key_align )        \
CC4_PADDING(                                                                      \
  el_size + CC4_MAP_EL_PADDING( el_size, key_align ) + key_size +                 \
  CC4_MAP_KEY_PADDING( el_size, key_size, key_align ) + sizeof( cc4_probelen_ty ), \
  CC4_MAX( key_align, CC4_MAX( el_align, alignof( cc4_probelen_ty ) ) )             \
)                                                                                \

#define CC4_SET_EL_PADDING( el_size )             \
CC4_PADDING( el_size, alignof( cc4_probelen_ty ) ) \

#define CC4_SET_PROBELEN_PADDING( el_size, el_align )                 \
CC4_PADDING(                                                          \
  el_size + CC4_SET_EL_PADDING( el_size ) + sizeof( cc4_probelen_ty ), \
  CC4_MAX( el_align, alignof( cc4_probelen_ty ) )                      \
)                                                                    \

// Struct for conveying key information from _Generic macro into below function.
typedef struct
{
  uint64_t size;
  uint64_t align;
} cc4_key_details_ty;

// Function for creating the uint64_t layout descriptor.
// This function must be inlined in order for layout calculations to be optimized into a compile-time constant.
#ifdef __GNUC__
__attribute__((always_inline))
#endif
static inline uint64_t cc4_layout( size_t cntr_id, uint64_t el_size, uint64_t el_align, cc4_key_details_ty key_details )
{
  if( cntr_id == CC4_MAP )
    return
      key_details.size                                                                        |
      CC4_MAP_EL_PADDING( el_size, key_details.align )                                   << 32 |
      CC4_MAP_KEY_PADDING( el_size, key_details.size, key_details.align )                << 40 |
      CC4_MAP_PROBELEN_PADDING( el_size, el_align, key_details.size, key_details.align ) << 48;

  if( cntr_id == CC4_SET )
    return
      el_size                                            |
      (uint64_t)0                                  << 32 |
      CC4_SET_EL_PADDING( el_size )                 << 40 |
      CC4_SET_PROBELEN_PADDING( el_size, el_align ) << 48;

  return 0; // Other container types don't require layout data.
}

// Macros for extracting data from layout descriptor.

#define CC4_KEY_SIZE( layout ) (uint32_t)( layout )

#define CC4_KEY_OFFSET( el_size, layout ) ( (el_size) + (uint8_t)( layout >> 32 ) )

#define CC4_PROBELEN_OFFSET( el_size, layout )                                           \
( CC4_KEY_OFFSET( el_size, layout ) + (uint32_t)( layout ) + (uint8_t)( layout >> 40 ) ) \

#define CC4_BUCKET_SIZE( el_size, layout )                                                        \
( CC4_PROBELEN_OFFSET( el_size, layout ) + sizeof( cc4_probelen_ty ) + (uint8_t)( layout >> 48 ) ) \

// Return type for all functions that could reallocate a container's memory.
// It contains a new container handle (the pointer may have changed to due reallocation) and an additional pointer whose
// purpose depends on the function.
// For functions that insert elements, that pointer points to the newly inserted elements, or NULL in the case of
// allocation failure.
// For other functions, that pointer is a dummy pointer that evaluates to true, or NULL in the case of allocation
// failure (i.e. its only purpose is to convey whether the operation was successful).
// The struct is aligned to max_align_t because the container handle must temporarily point to it (see
// CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT below).
typedef struct
{
  alignas( max_align_t )
  void *new_cntr;
  void *other_ptr;
} cc4_allocing_fn_result_ty;

// Helper function for one-line cc4_allocing_fn_result_ty.
static inline cc4_allocing_fn_result_ty cc4_make_allocing_fn_result( void *new_cntr, void *other_ptr )
{
  cc4_allocing_fn_result_ty result;
  result.new_cntr = new_cntr;
  result.other_ptr = other_ptr;
  return result;
}

// Performs memcpy and returns ptr.
// Used in conjunction with cc4_allocing_fn_result_ty (see below).
static inline void *cc4_memcpy_and_return_ptr( void *dest, void *src, size_t size, void *ptr )
{
  memcpy( dest, src, size );
  return ptr;
}

// All macros that call functions that could cause memory reallocation do two essential things to circumvent limitations
// of ISO C (such as the inability to declare variables in expressions and _Thread_local linking issues).
// Firstly, they temporarily set the container handle to point to a temporary cc4_insert_result object returned by the
// call to the function (see CC4_MAKE_LVAL_COPY above).
#define CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT( cntr, fn_call )                           \
  cntr = (CC4_TYPEOF_XP( cntr ))&CC4_MAKE_LVAL_COPY( cc4_allocing_fn_result_ty, fn_call ) \

// Secondly, they call cc4_memcpy_and_return_ptr to restore the correct container handle (a new pointer in the case of
// reallocation) and return a pointer, either to any new elements or to signify success/failure, to the user.
// (In the latter case, that pointer is cast to bool in the API macro before being passed to the user.)
// Without this call, we would be unable to access the new elements/success-or-failure pointer stored in the
// cc4_allocing_fn_result_ty after restoring the correct container handle.
// Note that outside the function, a temporary container handle is created from the new handle in the
// cc4_allocing_fn_result_ty so that the later (void *) is properly converted to the correct handle type.
// This new, correctly typed handle is then memcpy-ed over the user-supplied handle inside the function.
#define CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( cntr )                                              \
cc4_memcpy_and_return_ptr(                                                                     \
  &cntr,                                                                                      \
  &CC4_MAKE_LVAL_COPY( CC4_TYPEOF_XP( cntr ), ( (cc4_allocing_fn_result_ty *)cntr )->new_cntr ), \
  sizeof( cntr ),                                                                             \
  ( (cc4_allocing_fn_result_ty *)cntr )->other_ptr                                             \
)                                                                                             \

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                      Vector                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

// Vector header.
typedef struct
{
  alignas( max_align_t )
  size_t size;
  size_t cap;
} cc4_vec_hdr_ty;

// Global placeholder for vector with no allocated storage.
// In the case of vectors, the placeholder allows us to avoid checking for a NULL container handle inside functions.
static const cc4_vec_hdr_ty cc4_vec_placeholder = { 0, 0 };

// Easy header access function for internal use.
static inline cc4_vec_hdr_ty *cc4_vec_hdr( void *cntr )
{
  return (cc4_vec_hdr_ty *)cntr;
}

static inline size_t cc4_vec_size( void *cntr )
{
  return cc4_vec_hdr( cntr )->size;
}

static inline size_t cc4_vec_cap( void *cntr )
{
  return cc4_vec_hdr( cntr )->cap;
}

static inline bool cc4_vec_is_placeholder( void *cntr )
{
  return cc4_vec_hdr( cntr )->cap == 0;
}

// Returns a pointer-iterator to the element at a specified index.
static inline void *cc4_vec_get(
  void *cntr,
  void *key, /* Pointer to size_t index */
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( cc4_cmpr_fnptr_ty, cmpr )
)
{
  return (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * *(size_t *)key;
}

// Ensures that the capacity is large enough to support n elements without reallocation.
// Returns a cc4_allocing_fn_result_ty containing the new handle and a pointer that evaluates to true if the operation
// was successful.
static inline cc4_allocing_fn_result_ty cc4_vec_reserve(
  void *cntr,
  size_t n,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( double, max_load ),
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  if( cc4_vec_cap( cntr ) >= n )
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );

  bool is_placeholder = cc4_vec_is_placeholder( cntr );

  cc4_vec_hdr_ty *new_cntr = (cc4_vec_hdr_ty *)realloc_(
    is_placeholder ? NULL : cntr,
    sizeof( cc4_vec_hdr_ty ) + el_size * n
  );

  if( !new_cntr )
    return cc4_make_allocing_fn_result( cntr, NULL );

  if( is_placeholder )
    new_cntr->size = 0;

  new_cntr->cap = n;
  return cc4_make_allocing_fn_result( new_cntr, cc4_dummy_true_ptr );
}

// Inserts elements at the specified index.
// Returns a cc4_allocing_fn_result_ty containing the new handle and a pointer-iterator to the newly inserted elements.
// If the underlying storage needed to be expanded and an allocation failure occurred, or if n is zero, the latter
// pointer will be NULL.
static inline cc4_allocing_fn_result_ty cc4_vec_insert_n(
  void *cntr,
  size_t index,
  void *els,
  size_t n,
  size_t el_size,
  void *( *realloc_ )( void *, size_t )
)
{
  if( n == 0 )
    return cc4_make_allocing_fn_result( cntr, NULL );

  if( cc4_vec_size( cntr ) + n > cc4_vec_cap( cntr ) )
  {
    size_t cap = cc4_vec_cap( cntr );
    if( !cap )
      cap = 2;

    while( cap < cc4_vec_size( cntr ) + n )
      cap *= 2;

    cc4_allocing_fn_result_ty result = cc4_vec_reserve(
      cntr,
      cap,
      el_size,
      0, // Dummy.
      NULL,            // Dummy.
      0.0,             // Dummy.
      realloc_,
      NULL             // Dummy.
    );
    if( !result.other_ptr )
      return result;

    cntr = result.new_cntr;
  }

  char *new_els = (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * index;
  memmove( new_els + n * el_size, new_els, el_size * ( cc4_vec_hdr( cntr )->size - index ) );
  memcpy( new_els, els, el_size * n );
  cc4_vec_hdr( cntr )->size += n;

  return cc4_make_allocing_fn_result( cntr, new_els );
}

static inline cc4_allocing_fn_result_ty cc4_vec_insert(
  void *cntr,
  void *el,
  void *key, // Pointer to size_t index.
  CC4_UNUSED( bool, replace ),
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( cc4_cmpr_fnptr_ty, cmpr ),
  CC4_UNUSED( double, max_load ),
  CC4_UNUSED( cc4_dtor_fnptr_ty, el_dtor ),
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  return cc4_vec_insert_n( cntr, *(size_t *)key, el, 1, el_size, realloc_ );
}

static inline cc4_allocing_fn_result_ty cc4_vec_push_n(
  void *cntr,
  void *els,
  size_t n,
  size_t el_size,
  cc4_realloc_fnptr_ty realloc_
)
{
  return cc4_vec_insert_n( cntr, cc4_vec_size( cntr ), els, n, el_size, realloc_ );
}

static inline cc4_allocing_fn_result_ty cc4_vec_push(
  void *cntr,
  void *el,
  size_t el_size,
  cc4_realloc_fnptr_ty realloc_
)
{
  return cc4_vec_push_n( cntr, el, 1, el_size, realloc_ );
}

// Erases n elements at the specified index.
// Returns a pointer-iterator to the element after the erased elements, or an end pointer-iterator if there is no
// subsequent element.
static inline void *cc4_vec_erase_n(
  void *cntr,
  size_t index,
  size_t n,
  size_t el_size,
  cc4_dtor_fnptr_ty el_dtor
)
{
  if( n == 0 )
    return (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * index;

  if( el_dtor )
    for( size_t j = 0; j < n; ++j )
      el_dtor( (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * ( index + j ) );

  memmove(
    (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * index,
    (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * ( index + n ),
    ( cc4_vec_hdr( cntr )->size - n - index ) * el_size
  );

  cc4_vec_hdr( cntr )->size -= n;
  return (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * index;
}

// Shrinks vector's capacity to its current size.
// Returns a cc4_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline void *cc4_vec_erase(
  void *cntr,
  void *key, // Pointer to size_t index.
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( cc4_cmpr_fnptr_ty, cmpr ),
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  return cc4_vec_erase_n( cntr, *(size_t *)key, 1, el_size, el_dtor );
}

// Sets the number of elements in the vector.
// If n is below the current size, then the destructor is called for all erased elements.
// In this case, the vector's capacity is not changed.
// If n is above the current size, the new elements are uninitialized.
// Returns a cc4_allocing_fn_result_ty containing new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline cc4_allocing_fn_result_ty cc4_vec_resize(
  void *cntr,
  size_t n,
  size_t el_size,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_realloc_fnptr_ty realloc_
)
{
  // No resize necessary (also handles placeholder).
  if( n == cc4_vec_size( cntr ) )
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );

  // Downsizing.
  if( n < cc4_vec_size( cntr ) )
  {
    cc4_vec_erase_n( cntr, n, cc4_vec_size( cntr ) - n, el_size, el_dtor );
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );
  }

  // Up-sizing.
  cc4_allocing_fn_result_ty result = cc4_vec_reserve(
    cntr,
    n,
    el_size,
    0, // Dummy.
    NULL,            // Dummy.
    0.0,             // Dummy.
    realloc_,
    NULL             // Dummy.
  );
  if( !result.other_ptr )
    return result;

  cc4_vec_hdr( result.new_cntr )->size = n;

  return result;
}

static inline cc4_allocing_fn_result_ty cc4_vec_shrink(
  void *cntr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( double, max_load ),
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  if( cc4_vec_size( cntr ) == cc4_vec_cap( cntr ) ) // Also handles placeholder.
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );

  if( cc4_vec_size( cntr ) == 0 )
  {
    // Restore placeholder.
    free_( cntr );
    return cc4_make_allocing_fn_result( (void *)&cc4_vec_placeholder, cc4_dummy_true_ptr );
  }

  cc4_vec_hdr_ty *new_cntr = (cc4_vec_hdr_ty *)realloc_( cntr, sizeof( cc4_vec_hdr_ty ) + el_size * cc4_vec_size( cntr ) );
  if( !new_cntr )
    return cc4_make_allocing_fn_result( cntr, NULL );

  cc4_vec_hdr( new_cntr )->cap = cc4_vec_size( new_cntr );
  return cc4_make_allocing_fn_result( new_cntr, cc4_dummy_true_ptr );
}

// Initializes a shallow copy of the source vector.
// The capacity of the new vector is the size of the source vector, not its capacity.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc4_vec_init_clone(
  void *src,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  if( cc4_vec_size( src ) == 0 )
    return (void *)&cc4_vec_placeholder;

  cc4_allocing_fn_result_ty result = cc4_vec_resize(
    (void *)&cc4_vec_placeholder,
    cc4_vec_size( src ),
    el_size,
    NULL, // dtor unused.
    realloc_
  );

  if( !result.other_ptr )
    return NULL;

  memcpy(
    (char *)result.new_cntr + sizeof( cc4_vec_hdr_ty ),
    (char *)src + sizeof( cc4_vec_hdr_ty ),
    el_size * cc4_vec_size( src )
  );

  return result.new_cntr;
}

// Erases all elements, calling their destructors if necessary.
static inline void cc4_vec_clear(
  void *cntr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  cc4_vec_erase_n( cntr, 0, cc4_vec_size( cntr ), el_size, el_dtor );
}

// Clears the vector and frees its memory if it is not a placeholder.
static inline void cc4_vec_cleanup(
  void *cntr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_free_fnptr_ty free_
)
{
  cc4_vec_clear(
    cntr,
    el_size,
    0, // Dummy.
    el_dtor,
    NULL,            // Dummy.
    NULL             // Dummy.
  );

  if( !cc4_vec_is_placeholder( cntr ) )
    free_( cntr );
}

static inline void *cc4_vec_end(
  void *cntr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * cc4_vec_size( cntr );
}

static inline void *cc4_vec_next(
  CC4_UNUSED( void *, cntr ),
  void *itr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout )
)
{
  return (char *)itr + el_size;
}

static inline void *cc4_vec_first(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc4_vec_hdr_ty );
}

static inline void *cc4_vec_last(
  void *cntr,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout )
)
{
  return (char *)cntr + sizeof( cc4_vec_hdr_ty ) + el_size * ( cc4_vec_size( cntr ) - 1 );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        List                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/

// List is implemented a doubly linked list with sentinel nodes.

// Node header.
typedef struct cc4_listnode_hdr_ty
{
  alignas( max_align_t )
  struct cc4_listnode_hdr_ty *prev;
  struct cc4_listnode_hdr_ty *next;
} cc4_listnode_hdr_ty;

// List header.
// It does not need to be aligned to alignof( max_align_t ) because no memory is allocated after the header.
typedef struct
{
  size_t size;
  cc4_listnode_hdr_ty  r_end;
  cc4_listnode_hdr_ty  end;
} cc4_list_hdr_ty;

// Placeholder for list with no allocated header.
// The main purpose this serves is to provide every list with stable r_end and end iterators across translation units
// and irrespective of whether any memory has been allocated for its header.
// Every list initially (after cc4_init) points to this placeholder, which differs across translation units, and is then
// associated with that placeholder until cc4_cleanup is called.
// Calls to cc4_r_end and cc4_end on a list return pointers to the associated placeholder's r_end and end elements even
// after a header has been dynamically allocated to it.
// The way this works is that the placeholder's r_end.prev pointer points to the placeholder's r_end (i.e. a circular
// link), and ditto for end.next.
// Meanwhile, a list with an allocated header has it's r_end.prev pointer point to the placeholder's r_end, and ditto
// for it's end.next pointer.
// Therefore cc4_list_hdr( cntr )->r_end.prev always produces the associated placeholder's r_end, and ditto for
// cc4_list_hdr( cntr )->end.next.
// Unfortunately, this means that r_end and end must be handled as special cases during inserts, splices, and iteration.
const static cc4_list_hdr_ty cc4_list_placeholder = {
  0,
  {
    (cc4_listnode_hdr_ty *)&cc4_list_placeholder.r_end, // Circular link.
    (cc4_listnode_hdr_ty *)&cc4_list_placeholder.end
  },
  {
    (cc4_listnode_hdr_ty *)&cc4_list_placeholder.r_end,
    (cc4_listnode_hdr_ty *)&cc4_list_placeholder.end    // Circular link.
  }
};

// Easy access to list header.
static inline cc4_list_hdr_ty *cc4_list_hdr( void *cntr )
{
  return (cc4_list_hdr_ty *)cntr;
}

// Easy access to a list node header from a pointer-iterator.
static inline cc4_listnode_hdr_ty *cc4_listnode_hdr( void *itr )
{
  return (cc4_listnode_hdr_ty *)( (char *)itr - sizeof( cc4_listnode_hdr_ty ) );
}

// Easy access to a pointer-iterator from pointer to a list node header.
static inline void *cc4_list_el( void *hdr )
{
  return (char *)hdr + sizeof( cc4_listnode_hdr_ty );
}

static inline bool cc4_list_is_placeholder( void *cntr )
{
  return cc4_list_hdr( cntr )->r_end.prev == &cc4_list_hdr( cntr )->r_end;
}

static inline size_t cc4_list_size( void *cntr )
{
  return cc4_list_hdr( cntr )->size;
}

static inline void *cc4_list_r_end( void *cntr )
{
  return cc4_list_el( cc4_list_hdr( cntr )->r_end.prev );
}

static inline void *cc4_list_end(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  return cc4_list_el( cc4_list_hdr( cntr )->end.next );
}

static inline void *cc4_list_prev(
  void *cntr,
  void *itr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  cc4_listnode_hdr_ty *prev = cc4_listnode_hdr( itr )->prev;

  // If itr is r_end, then we need to decrement the iterator once more to ensure that the returned iterator is the r_end
  // of the placeholder originally associated with the list.
  if( prev == &cc4_list_hdr( cntr )->r_end )
    prev = prev->prev;

  return cc4_list_el( prev );
}

static inline void *cc4_list_next(
  void *cntr,
  void *itr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  cc4_listnode_hdr_ty *next = cc4_listnode_hdr( itr )->next;

  // See comment in cc4_list_prev above.
  if( next == &cc4_list_hdr( cntr )->end )
    next = next->next;

  return cc4_list_el( next );
}

static inline void *cc4_list_first(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  return cc4_list_next(
    cntr,
    cc4_list_el( &cc4_list_hdr( cntr )->r_end ),
    0,              // Dummy.
    0 // Dummy.
  );
}

static inline void *cc4_list_last(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout )
)
{
  return cc4_list_prev(
    cntr,
    cc4_list_el( &cc4_list_hdr( cntr )->end ),
    0,              // Dummy.
    0 // Dummy.
  );
}

// Allocates a header for a list that is currently a placeholder.
// Returns the new container handle, or NULL in the case of allocation failure.
static inline void *cc4_list_alloc_hdr(
  void *cntr,
  cc4_realloc_fnptr_ty realloc_
)
{
  cc4_list_hdr_ty *new_cntr = (cc4_list_hdr_ty *)realloc_( NULL, sizeof( cc4_list_hdr_ty ) );
  if( !new_cntr )
    return NULL;

  new_cntr->r_end.next = &new_cntr->end;
  new_cntr->end.prev = &new_cntr->r_end;

  // Link the new header's r_end and end with the original placeholder's r_end and end.
  new_cntr->r_end.prev = &cc4_list_hdr( cntr )->r_end;
  new_cntr->end.next = &cc4_list_hdr( cntr )->end;

  new_cntr->size = 0; 
  return new_cntr;
}

// Attaches a node to the list before the node pointed to by the specified pointer-iterator.
static inline void cc4_list_attach( void *cntr, void *itr, cc4_listnode_hdr_ty *node )
{
  // Handle r_end and end iterators as a special case.
  // We need to convert the iterator from the global placeholder's r_end or end to the local r_end or end.
  if( itr == cc4_list_r_end( cntr ) )
    itr = cc4_list_el( &cc4_list_hdr( cntr )->r_end );
  else if( itr == cc4_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
    itr = cc4_list_el( &cc4_list_hdr( cntr )->end );

  // Link node.
  node->next = cc4_listnode_hdr( itr );
  node->prev = node->next->prev;
  node->next->prev = node;
  node->prev->next = node;
}

// Inserts an element into the list before the node pointed to by a given pointer-iterator.
// Returns a cc4_allocing_fn_result_ty containing the new container handle and a pointer-iterator to the newly inserted
// element (or NULL in the case of allocation failure).
static inline cc4_allocing_fn_result_ty cc4_list_insert(
  void *cntr,
  void *el,
  void *key, // Pointer to void pointer-interator.
  CC4_UNUSED( bool, replace ),
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( cc4_cmpr_fnptr_ty, cmpr ),
  CC4_UNUSED( double, max_load ),
  CC4_UNUSED( cc4_dtor_fnptr_ty, el_dtor ),
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  if( cc4_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc4_list_alloc_hdr( cntr, realloc_ );
    if( !new_cntr )
      return cc4_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc4_listnode_hdr_ty *new_node = (cc4_listnode_hdr_ty *)realloc_( NULL, sizeof( cc4_listnode_hdr_ty ) + el_size );
  if( !new_node )
    return cc4_make_allocing_fn_result( cntr, NULL );

  memcpy( cc4_list_el( new_node ), el, el_size );

  // Handle r_end and end iterators as a special case.
  // We need to convert the iterator from the associated placeholder's r_end or end to the local r_end or end.
  if( *(void **)key == cc4_list_r_end( cntr ) )
    *(void **)key = cc4_list_el( &cc4_list_hdr( cntr )->r_end );
  else if( *(void **)key == cc4_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
    *(void **)key = cc4_list_el( &cc4_list_hdr( cntr )->end );

  cc4_list_attach( cntr, *(void **)key, new_node );

  ++cc4_list_hdr( cntr )->size;

  return cc4_make_allocing_fn_result( cntr, cc4_list_el( new_node ) );  
}

static inline cc4_allocing_fn_result_ty cc4_list_push(
  void *cntr,
  void *el,
  size_t el_size,
  cc4_realloc_fnptr_ty realloc_
)
{
  return cc4_list_insert(
    cntr,
    el,
    &CC4_MAKE_LVAL_COPY( void *, cc4_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ ) ),
    false,           // Dummy.
    el_size,
    0, // Dummy.
    NULL,            // Dummy.
    NULL,            // Dummy.
    0.0,             // Dummy.
    NULL,            // Dummy.
    NULL,            // Dummy.
    realloc_,
    NULL             // Dummy.
  );
}

// Erases the element pointed to by a given pointer-iterator and returns a pointer-iterator to next element (or end if
// the element was the last element in the list).
static inline void *cc4_list_erase(
  void *cntr,
  void *key, // Pointer to void pointer-interator.
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout ),
  CC4_UNUSED( cc4_hash_fnptr_ty, hash ),
  CC4_UNUSED( cc4_cmpr_fnptr_ty, cmpr ),
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_free_fnptr_ty free_
)
{
  cc4_listnode_hdr_ty *hdr = cc4_listnode_hdr( *(void **)key );
  cc4_listnode_hdr_ty *next = hdr->next;
  hdr->prev->next = next;
  next->prev = hdr->prev;
  
  if( el_dtor )
    el_dtor( *(void **)key );

  free_( hdr );
  --cc4_list_hdr( cntr )->size;

  // If next is end, we need to make sure we're returning the associated placeholder's end.
  if( next == &cc4_list_hdr( cntr )->end )
    next = next->next;

  return cc4_list_el( next );
}

// Removes the element pointed to by pointer-iterator src_itr from the source list and attaches it to the list before
// pointer-iterator itr.
// Although this function never allocates memory for the element/node itself, it must allocate the list's header if the
// list is currently a placeholder.
// Returns a cc4_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful or false in the case of allocation failure.
static inline cc4_allocing_fn_result_ty cc4_list_splice(
  void *cntr,
  void *itr,
  void *src,
  void *src_itr,
  cc4_realloc_fnptr_ty realloc_
)
{
  if( cc4_list_is_placeholder( cntr ) )
  {
    void *new_cntr = cc4_list_alloc_hdr( cntr, realloc_ );
    if( !new_cntr )
      return cc4_make_allocing_fn_result( cntr, NULL );

    cntr = new_cntr;
  }

  cc4_listnode_hdr( src_itr )->prev->next = cc4_listnode_hdr( src_itr )->next;
  cc4_listnode_hdr( src_itr )->next->prev = cc4_listnode_hdr( src_itr )->prev;
  cc4_list_attach( cntr, itr, cc4_listnode_hdr( src_itr ) );

  --cc4_list_hdr( src )->size;
  ++cc4_list_hdr( cntr )->size;

  return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );
}

// Initializes a shallow copy of the source list.
// This requires allocating memory for every node, as well as for the list's header unless src is a placeholder.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc4_list_init_clone(
  void *src,
  size_t el_size,
  CC4_UNUSED( uint64_t, layout ),
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  cc4_allocing_fn_result_ty result = { (void *)&cc4_list_placeholder, cc4_dummy_true_ptr };

  for(
    void *i = cc4_list_first( src, 0 /* Dummy */, 0 /* Dummy */ );
    i != cc4_list_end( src, 0 /* Dummy */, 0 /* Dummy */ );
    i = cc4_list_next( src, i, 0 /* Dummy */, 0 /* Dummy */ )
  )
  {
    result = cc4_list_insert(
      result.new_cntr,
      i,
      &CC4_MAKE_LVAL_COPY(
        void *,
        cc4_list_end( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ )
      ),
      false,           // Dummy.
      el_size,
      0, // Dummy.
      NULL,            // Dummy.
      NULL,            // Dummy.
      0.0,             // Dummy.
      NULL,            // Dummy.
      NULL,            // Dummy.
      realloc_,
      NULL             // Dummy.
    );

    if( !result.other_ptr )
    {
      // Erase incomplete clone without invoking destructors.
      
      void *j = cc4_list_first( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ );
      while( j != cc4_list_end( result.new_cntr, 0 /* Dummy */, 0 /* Dummy */ ) )
      {
        void *next = cc4_list_next( result.new_cntr, j, 0 /* Dummy */, 0 /* Dummy */ );
        free_( cc4_listnode_hdr( j ) );
        j = next;
      }

      if( !cc4_list_is_placeholder( result.new_cntr ) )
        free_( result.new_cntr );

      return NULL;
    }
  }

  return result.new_cntr;
}

// Erases all elements, calling their destructors if necessary.
static inline void cc4_list_clear(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout ),
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_free_fnptr_ty free_
)
{
  while(
    cc4_list_first( cntr, 0 /* Dummy */, 0 /* Dummy */ ) !=
    cc4_list_end( cntr, 0 /* Dummy */, 0 /* Dummy */ )
  )
    cc4_list_erase(
      cntr,
      &CC4_MAKE_LVAL_COPY( void *, cc4_list_first( cntr, 0 /* Dummy */, 0 /* Dummy */ ) ),
      0,               // Dummy.
      0, // Dummy.
      NULL,            // Dummy.
      NULL,            // Dummy.
      el_dtor,
      NULL,            // Dummy.
      free_
    );
}

// Erases all elements, calling their destructors if necessary, and frees memory for the list's header if it is not
// a placeholder.
static inline void cc4_list_cleanup(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  CC4_UNUSED( uint64_t, layout ),
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  cc4_free_fnptr_ty free_
)
{
  cc4_list_clear( cntr, 0 /* Dummy */, 0 /* Dummy */, el_dtor, NULL /* Dummy */, free_ );

  if( !cc4_list_is_placeholder( cntr ) )
    free_( cntr );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Map                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Map header.
typedef struct
{
  alignas( max_align_t )
  size_t size;
  size_t cap;
} cc4_map_hdr_ty;

// Placeholder for map with no allocated memory.
// In the case of maps, this placeholder allows us to avoid checking for a NULL handle inside functions.
static const cc4_map_hdr_ty cc4_map_placeholder = { 0, 0 };

// Easy header access function for internal use.
static inline cc4_map_hdr_ty *cc4_map_hdr( void *cntr )
{
  return (cc4_map_hdr_ty *)cntr;
}

// Size and capacity.

static inline size_t cc4_map_size( void *cntr )
{
  return cc4_map_hdr( cntr )->size;
}

static inline size_t cc4_map_cap( void *cntr )
{
  return cc4_map_hdr( cntr )->cap;
}

static inline bool cc4_map_is_placeholder( void *cntr )
{
  return cc4_map_cap( cntr ) == 0;
}

// Functions for easily accessing element, key, and probe length for the bucket at index i.
// The element pointer also denotes the beginning of the bucket.

static inline void *cc4_map_el( void *cntr, size_t i, size_t el_size, uint64_t layout )
{
  return (char *)cntr + sizeof( cc4_map_hdr_ty ) + CC4_BUCKET_SIZE( el_size, layout ) * i;
}

static inline void *cc4_map_key( void *cntr, size_t i, size_t el_size, uint64_t layout )
{
  return (char *)cc4_map_el( cntr, i, el_size, layout ) + CC4_KEY_OFFSET( el_size, layout );
}

static inline cc4_probelen_ty *cc4_map_probelen( void *cntr, size_t i, size_t el_size, uint64_t layout )
{
  return (cc4_probelen_ty *)( (char *)cc4_map_el( cntr, i, el_size, layout ) + CC4_PROBELEN_OFFSET( el_size, layout ) );
}

// Inserts an element into the map.
// Assumes that the map has empty slots and therefore that failure cannot occur (hence the "raw" label).
// If replace is true, then el will replace any existing element with the same key.
// Returns a pointer-iterator to the newly inserted element, or to the existing element with the same key if replace is
// false.
// For the exact mechanics of Robin-Hood hashing, see Sebastian Sylvan's helpful article:
// www.sebastiansylvan.com/post/robin-hood-hashing-should-be-your-default-hash-table-implementation
// However, this function includes an optimization not mentioned in descriptions of Robin Hood hashing.
// Specifically, it enters a second, inner loop once a swap occurs.
// This allows us to eliminate some checks and branching based on whether the element to insert has already been placed,
// albeit at the cost of longer code.
static inline void *cc4_map_insert_raw(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor
)
{
  size_t i = hash( key ) & ( cc4_map_hdr( cntr )->cap - 1 );
  cc4_probelen_ty probelen = 1;

  while( true )
  {
    if( probelen > *cc4_map_probelen( cntr, i, el_size, layout ) )
    {
      // Empty bucket, or stealing occupied bucket.
      void *to_return = cc4_map_el( cntr, i, el_size, layout );
      ++cc4_map_hdr( cntr )->size;

      while( true )
      {
        if( !*cc4_map_probelen( cntr, i, el_size, layout ) )
        {
          memcpy( cc4_map_key( cntr, i, el_size, layout ), key, CC4_KEY_SIZE( layout ) );
          memcpy( cc4_map_el( cntr, i, el_size, layout ), el, el_size );
          *cc4_map_probelen( cntr, i, el_size, layout ) = probelen;
          return to_return;
        }

        if( probelen > *cc4_map_probelen( cntr, i, el_size, layout ) )
        {
          CC4_MEMSWAP( key, cc4_map_key( cntr, i, el_size, layout ), CC4_KEY_SIZE( layout ) );
          CC4_MEMSWAP( el, cc4_map_el( cntr, i, el_size, layout ), el_size );

          cc4_probelen_ty temp_probelen = *cc4_map_probelen( cntr, i, el_size, layout );
          *cc4_map_probelen( cntr, i, el_size, layout ) = probelen;
          probelen = temp_probelen;
        }

        i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
        ++probelen;
      }
    }
    else if(
      probelen == *cc4_map_probelen( cntr, i, el_size, layout ) &&
      cmpr( cc4_map_key( cntr, i, el_size, layout ), key ) == 0
    )
    {
      // Same key.

      if( replace )
      {
        if( key_dtor )
          key_dtor( cc4_map_key( cntr, i, el_size, layout ) );

        if( el_dtor )
          el_dtor( cc4_map_el( cntr, i, el_size, layout ) );

        memcpy( cc4_map_key( cntr, i, el_size, layout ), key, CC4_KEY_SIZE( layout ) );
        memcpy( cc4_map_el( cntr, i, el_size, layout ), el, el_size );
      }

      return cc4_map_el( cntr, i, el_size, layout );
    }

    i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }
}

// Same as previous function, except for elements with keys known not to already exist in the map.
// This function is used for rehashing when the map's capacity changes.
// When we known that the key is new, we can skip certain checks and achieve a small performance improvement.
static inline void *cc4_map_insert_raw_unique(
  void *cntr,
  void *el,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash
)
{
  size_t i = hash( key ) & ( cc4_map_hdr( cntr )->cap - 1 );
  cc4_probelen_ty probelen = 1;

  while( true )
  {
    if( probelen > *cc4_map_probelen( cntr, i, el_size, layout ) )
    {
      // Empty bucket, or stealing occupied bucket.
      void *to_return = cc4_map_el( cntr, i, el_size, layout );
      ++cc4_map_hdr( cntr )->size;

      while( true )
      {
        if( !*cc4_map_probelen( cntr, i, el_size, layout ) )
        {
          memcpy( cc4_map_key( cntr, i, el_size, layout ), key, CC4_KEY_SIZE( layout ) );
          memcpy( cc4_map_el( cntr, i, el_size, layout ), el, el_size );
          *cc4_map_probelen( cntr, i, el_size, layout ) = probelen;
          return to_return;
        }

        if( probelen > *cc4_map_probelen( cntr, i, el_size, layout ) )
        {
          CC4_MEMSWAP( key, cc4_map_key( cntr, i, el_size, layout ), CC4_KEY_SIZE( layout ) );
          CC4_MEMSWAP( el, cc4_map_el( cntr, i, el_size, layout ), el_size );

          cc4_probelen_ty temp_probelen = *cc4_map_probelen( cntr, i, el_size, layout );
          *cc4_map_probelen( cntr, i, el_size, layout ) = probelen;
          probelen = temp_probelen;
        }

        i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
        ++probelen;
      }
    }

    i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }
}

// Returns the minimum capacity required to accommodate n elements, which is governed by the max load factor associated
// with the map's key type.
static inline size_t cc4_map_min_cap_for_n_els( size_t n, double max_load )
{
  if( n == 0 )
    return 0;

  // Round up to power of 2.
  size_t cap = 8;
  while( n > cap * max_load )
    cap *= 2;

  return cap;
}

// Creates a rehashed duplicate of cntr with capacity cap.
// Assumes that cap is large enough to accommodate all elements in cntr without violating the max load factor.
// Returns pointer to the duplicate, or NULL in the case of allocation failure.
static inline void *cc4_map_make_rehash(
  void *cntr,
  size_t cap,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_realloc_fnptr_ty realloc_
)
{
  cc4_map_hdr_ty *new_cntr = (cc4_map_hdr_ty *)realloc_(
    NULL,
    sizeof( cc4_map_hdr_ty ) + CC4_BUCKET_SIZE( el_size, layout ) * cap
  );
  if( !new_cntr )
    return NULL;

  new_cntr->size = 0;
  new_cntr->cap = cap;
  for( size_t i = 0; i < cap; ++i )
    *cc4_map_probelen( new_cntr, i, el_size, layout ) = 0;

  for( size_t i = 0; i < cc4_map_hdr( cntr )->cap; ++i )
    if( *cc4_map_probelen( cntr, i, el_size, layout ) )
      cc4_map_insert_raw_unique(
        new_cntr,
        cc4_map_el( cntr, i, el_size, layout ),
        cc4_map_key( cntr, i, el_size, layout ),
        el_size,
        layout,
        hash
      );

  return new_cntr;
}

// Reserves capacity such that the map can accommodate n elements without reallocation (i.e. without violating the
// max load factor).
// Returns a cc4_allocing_fn_result_ty containing new container handle and a pointer that evaluates to true if the
// operation successful or false in the case of allocation failure.
static inline cc4_allocing_fn_result_ty cc4_map_reserve(
  void *cntr,
  size_t n,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  double max_load,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  size_t cap = cc4_map_min_cap_for_n_els( n, max_load );

  if( cc4_map_cap( cntr ) >= cap )
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );

  void *new_cntr = cc4_map_make_rehash(
    cntr,
    cap,
    el_size,
    layout,
    hash,
    realloc_
  );
  if( !new_cntr )
    return cc4_make_allocing_fn_result( cntr, NULL );

  if( !cc4_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc4_make_allocing_fn_result( new_cntr, cc4_dummy_true_ptr );
}

// Inserts an element.
// If replace is true, then el replaces any existing element with the same key.
// If the map exceeds its load factor, the underlying storage is expanded and a complete rehash occurs.
// Returns a cc4_allocing_fn_result_ty containing the new container handle and a pointer to the newly inserted element,
// or to the existing element with the same key if replace is false.
// If the underlying storage needed to be expanded and an allocation failure occurred, the latter pointer will be NULL.
// This function checks to ensure that the map could accommodate an insertion before searching for the existing element.
// Therefore, failure can occur even if an element with the same key already exists and no reallocation was actually
// necessary.
// This was a design choice in favor of code simplicity and readability over ideal behavior in a corner case.
static inline cc4_allocing_fn_result_ty cc4_map_insert(
  void *cntr,
  void *el,
  void *key,
  bool replace,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr,
  double max_load,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  if( cc4_map_size( cntr ) + 1 > cc4_map_cap( cntr ) * max_load )
  {
    cc4_allocing_fn_result_ty result = cc4_map_reserve(
      cntr,
      cc4_map_size( cntr ) + 1,
      el_size,
      layout,
      hash,
      max_load,
      realloc_,
      free_
    );

    if( !result.other_ptr )
      return result;

    cntr = result.new_cntr;
  }

  void *new_el = cc4_map_insert_raw(
    cntr,
    el,
    key,
    replace,
    el_size,
    layout,
    hash,
    cmpr,
    el_dtor,
    key_dtor
  );

  return cc4_make_allocing_fn_result( cntr, new_el );
}

// Returns a pointer-iterator to the element with the specified key, or NULL if no such element exists.
static inline void *cc4_map_get(
  void *cntr,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr
)
{
  if( cc4_map_size( cntr ) == 0 )
    return NULL;

  size_t i = hash( key ) & ( cc4_map_hdr( cntr )->cap - 1 );
  cc4_probelen_ty probelen = 1;

  while( probelen <= *cc4_map_probelen( cntr, i, el_size, layout ) )
  {
    if(
      probelen == *cc4_map_probelen( cntr, i, el_size, layout ) &&
      cmpr( cc4_map_key( cntr, i, el_size, layout ), key ) == 0
    )
      return cc4_map_el( cntr, i, el_size, layout );

    i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }

  return NULL;
}

// Returns a pointer to the key for the element pointed to by the specified pointer-iterator.
static inline void *cc4_map_key_for(
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  return (char *)itr + CC4_KEY_OFFSET( el_size, layout );
}

// Erases the element pointer to by pointer-iterator itr.
// For the exact mechanics of erasing elements in a Robin-Hood hash table, see Sebastian Sylvan's:
// www.sebastiansylvan.com/post/more-on-robin-hood-hashing-2/
static inline void cc4_map_erase_itr(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor
)
{
  size_t i = ( (char *)itr - (char *)cc4_map_el( cntr, 0, el_size, layout ) ) / CC4_BUCKET_SIZE( el_size, layout );
  *cc4_map_probelen( cntr, i, el_size, layout ) = 0;
  --cc4_map_hdr( cntr )->size;

  if( key_dtor )
    key_dtor( cc4_map_key( cntr, i, el_size, layout ) );

  if( el_dtor )
    el_dtor( cc4_map_el( cntr, i, el_size, layout ) );

  while( true )
  {
    size_t next = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
    if( *cc4_map_probelen( cntr, next, el_size, layout ) <= 1 )
      break; // Empty slot or key already in its home bucket, so all done.
    
    //Bump backwards.

    memcpy(
      cc4_map_key( cntr, i, el_size, layout ),
      cc4_map_key( cntr, next, el_size, layout ),
      CC4_KEY_SIZE( layout )
    );
    memcpy( cc4_map_el( cntr, i, el_size, layout ), cc4_map_el( cntr, next, el_size, layout ), el_size );

    *cc4_map_probelen( cntr, i, el_size, layout ) =
      *cc4_map_probelen( cntr, next, el_size, layout ) - 1;
    *cc4_map_probelen( cntr, next, el_size, layout ) = 0;

    i = next;
  }
}

// Erases the element with the specified key, if it exists.
// Returns a pointer that evaluates to true if an element was erased, or else is NULL.
// This pointer is eventually cast to bool by the cc4_erase API macro.
static inline void *cc4_map_erase(
  void *cntr,
  void *key,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor,
  cc4_free_fnptr_ty free_
)
{
  if( cc4_map_size( cntr ) == 0 )
    return NULL;

  size_t i = hash( key ) & ( cc4_map_hdr( cntr )->cap - 1 );
  cc4_probelen_ty probelen = 1;

  while( probelen <= *cc4_map_probelen( cntr, i, el_size, layout ) )
  {
    if(
      probelen == *cc4_map_probelen( cntr, i, el_size, layout ) &&
      cmpr( cc4_map_key( cntr, i, el_size, layout ), key ) == 0
    )
    {
      cc4_map_erase_itr(
        cntr,
        cc4_map_el( cntr, i, el_size, layout ),
        el_size,
        layout,
        el_dtor,
        key_dtor
      );

      return cc4_dummy_true_ptr;
    }

    i = ( i + 1 ) & ( cc4_map_hdr( cntr )->cap - 1 );
    ++probelen;
  }

  return NULL;
}

// Shrinks map's capacity to the minimum possible without violating the max load factor associated with the key type.
// If shrinking is necessary, then a complete rehash occurs.
// Returns a cc4_allocing_fn_result_ty containing the new container handle and a pointer that evaluates to true if the
// operation was successful and false in the case of allocation failure.
static inline cc4_allocing_fn_result_ty cc4_map_shrink(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  double max_load,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  size_t cap = cc4_map_min_cap_for_n_els( cc4_map_size( cntr ), max_load );

  if( cap == cc4_map_cap( cntr ) ) // Shrink unnecessary.
    return cc4_make_allocing_fn_result( cntr, cc4_dummy_true_ptr );

  if( cap == 0 ) // Restore placeholder.
  {
    if( !cc4_map_is_placeholder( cntr ) )
      free_( cntr );

    return cc4_make_allocing_fn_result( (void *)&cc4_map_placeholder, cc4_dummy_true_ptr );
  }

  void *new_cntr = cc4_map_make_rehash(
    cntr,
    cap,
    el_size,
    layout,
    hash,
    realloc_
  );
  if( !new_cntr )
    return cc4_make_allocing_fn_result( cntr, NULL );

  if( !cc4_map_is_placeholder( cntr ) )
    free_( cntr );

  return cc4_make_allocing_fn_result( new_cntr, cc4_dummy_true_ptr );
}

// Initializes a shallow copy of the source map.
// The capacity of the copy is the same as the capacity of the source map, unless the source map is empty, in which case
// the copy is a placeholder.
// Hence, this function does no rehashing.
// Returns a the pointer to the copy, or NULL in the case of allocation failure.
// That return value is cast to bool in the corresponding macro.
static inline void *cc4_map_init_clone(
  void *src,
  size_t el_size,
  uint64_t layout,
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  if( cc4_map_size( src ) == 0 ) // Also handles placeholder.
    return (void *)&cc4_map_placeholder;

  cc4_map_hdr_ty *new_cntr = (cc4_map_hdr_ty*)realloc_(
    NULL,
    sizeof( cc4_map_hdr_ty ) + CC4_BUCKET_SIZE( el_size, layout ) * cc4_map_cap( src )
  );
  if( !new_cntr )
    return NULL;

  memcpy( new_cntr, src, sizeof( cc4_map_hdr_ty ) + CC4_BUCKET_SIZE( el_size, layout ) * cc4_map_cap( src ) );
  return new_cntr;
}

// Erases all elements, calling the destructors for the key and element types if necessary, without changing the map's
// capacity.
static inline void cc4_map_clear(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  if( cc4_map_size( cntr ) == 0 ) // Also handles placeholder map.
    return;

  for( size_t i = 0; i < cc4_map_hdr( cntr )->cap; ++i )
    if( *cc4_map_probelen( cntr, i, el_size, layout ) )
    {
      if( key_dtor )
        key_dtor( cc4_map_key( cntr, i, el_size, layout ) );

      if( el_dtor )
        el_dtor( cc4_map_el( cntr, i, el_size, layout ) );

      *cc4_map_probelen( cntr, i, el_size, layout ) = 0;
    }

  cc4_map_hdr( cntr )->size = 0;
}

// Clears the map and frees its memory if is not a placeholder.
static inline void cc4_map_cleanup(
  void *cntr,
  size_t el_size,
  uint64_t layout,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_dtor_fnptr_ty key_dtor,
  cc4_free_fnptr_ty free_
)
{
  cc4_map_clear( cntr, el_size, layout, key_dtor, el_dtor, NULL /* Dummy */ );

  if( !cc4_map_is_placeholder( cntr ) )
    free_( cntr );
}

// For maps, the container handle doubles up as r_end.
static inline void *cc4_map_r_end(
  void *cntr
)
{
  return cntr;
}

// Returns a pointer-iterator to the end of the bucket array.
static inline void *cc4_map_end(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  return cc4_map_el( cntr, cc4_map_hdr( cntr )->cap, el_size, layout );
}

// Returns a pointer-iterator to the first element, or end if the map is empty.
static inline void *cc4_map_first(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  for( size_t i = 0; i < cc4_map_hdr( cntr )->cap; ++i )
    if( *cc4_map_probelen( cntr, i, el_size, layout ) )
      return cc4_map_el( cntr, i, el_size, layout );

  return cc4_map_end( cntr, 0 /* Dummy */, layout );
}

// Returns a pointer-iterator to the last element, or r_end if the map is empty.
static inline void *cc4_map_last(
  void *cntr,
  size_t el_size,
  uint64_t layout
)
{
  for( size_t i = cc4_map_hdr( cntr )->cap; i-- > 0; )
    if( *cc4_map_probelen( cntr, i, el_size, layout ) )
      return cc4_map_el( cntr, i, el_size, layout );

  return cc4_map_r_end( cntr );
}

static inline void *cc4_map_prev(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  size_t j = ( (char *)itr - (char *)cc4_map_el( cntr, 0, el_size, layout ) ) / CC4_BUCKET_SIZE( el_size, layout );

  while( true )
  {
    if( j == 0 )
      return cc4_map_r_end( cntr );

    --j;
    if( *cc4_map_probelen( cntr, j, el_size, layout ) )
      return cc4_map_el( cntr, j, el_size, layout );
  }
}

static inline void *cc4_map_next(
  void *cntr,
  void *itr,
  size_t el_size,
  uint64_t layout
)
{
  size_t j = ( (char *)itr - (char *)cc4_map_el( cntr, 0, el_size, layout ) ) / CC4_BUCKET_SIZE( el_size, layout ) + 1;

  while( j < cc4_map_hdr( cntr )->cap && !*cc4_map_probelen( cntr, j, el_size, layout ) )
    ++j;

  return cc4_map_el( cntr, j, el_size, layout );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        Set                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// A set is implemented as a map where the key and element are combined into one space in memory.
// Hence, it reuses the functions for map, except:
//   - The key offset inside the bucket is zero.
//     This is handled at the API-macro level via the cc4_layout function and associated macros.
//   - The element size passed into map functions is zero in order to avoid double-memcpying.

static inline size_t cc4_set_size( void *cntr )
{
  return cc4_map_size( cntr );
}

static inline size_t cc4_set_cap( void *cntr )
{
  return cc4_map_cap( cntr );
}

static inline cc4_allocing_fn_result_ty cc4_set_reserve(
  void *cntr,
  size_t n,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  double max_load,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  return cc4_map_reserve( cntr, n, 0 /* Zero element size */, layout, hash, max_load, realloc_, free_ );
}

static inline cc4_allocing_fn_result_ty cc4_set_insert(
  void *cntr,
  void *key,
  bool replace,
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr,
  double max_load,
  cc4_dtor_fnptr_ty el_dtor,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  return cc4_map_insert(
    cntr,
    cntr,     // Dummy pointer for element as memcpying to a NULL pointer is undefined behavior even when size is zero.
    key,
    replace,
    0,        // Zero element size.
    layout,
    hash,
    cmpr,
    max_load,
    el_dtor,
    NULL,     // Only one dtor.
    realloc_,
    free_
  );
}

static inline void *cc4_set_get(
  void *cntr,
  void *key,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr
)
{
  return cc4_map_get( cntr, key, 0 /* Dummy */, layout, hash, cmpr );
}

static inline void cc4_set_erase_itr(
  void *cntr,
  void *itr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor )
)
{
  cc4_map_erase_itr( cntr, itr, 0 /* Zero element size */, layout, el_dtor, NULL /* Only one dtor */ );
}

static inline void *cc4_set_erase(
  void *cntr,
  void *key,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  cc4_cmpr_fnptr_ty cmpr,
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  return cc4_map_erase(
    cntr,
    key,
    0,       // Zero element size.
    layout,
    hash,
    cmpr,
    el_dtor,
    NULL,    // Only one dtor.
    NULL     // Dummy.
  );
}

static inline cc4_allocing_fn_result_ty cc4_set_shrink(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_hash_fnptr_ty hash,
  double max_load,
  cc4_realloc_fnptr_ty realloc_,
  cc4_free_fnptr_ty free_
)
{
  return cc4_map_shrink( cntr, 0 /* Zero element size */, layout, hash, max_load, realloc_, free_ );
}

static inline void *cc4_set_init_clone(
  void *src,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_realloc_fnptr_ty realloc_,
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  return cc4_map_init_clone( src, /* Zero element size */ 0, layout, realloc_, NULL /* Dummy */ );
}

static inline void cc4_set_clear(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  cc4_dtor_fnptr_ty el_dtor,
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  CC4_UNUSED( cc4_free_fnptr_ty, free_ )
)
{
  cc4_map_clear( cntr, 0 /* Zero element size */, layout, el_dtor, NULL /* Only one dtor */, NULL /* Dummy */ );
}

static inline void cc4_set_cleanup(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout,
  void ( *el_dtor )( void * ),
  CC4_UNUSED( cc4_dtor_fnptr_ty, key_dtor ),
  void ( *free_ )( void * ))
{
  cc4_map_cleanup( cntr, 0 /* Zero element size */, layout, el_dtor, NULL /* Only one dtor */, free_ );
}

static inline void *cc4_set_r_end( void *cntr )
{
  return cc4_map_r_end( cntr );
}

static inline void *cc4_set_end(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc4_map_end( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc4_set_first(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc4_map_first( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc4_set_last(
  void *cntr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc4_map_last( cntr, /* Zero element size */ 0, layout );
}

static inline void *cc4_set_prev(
  void *cntr,
  void *itr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc4_map_prev( cntr, itr, /* Zero element size */ 0, layout );
}

static inline void *cc4_set_next(
  void *cntr,
  void *itr,
  CC4_UNUSED( size_t, el_size ),
  uint64_t layout
)
{
  return cc4_map_next( cntr, itr, /* Zero element size */ 0, layout );
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*                                                        API                                                         */
/*--------------------------------------------------------------------------------------------------------------------*/

#define cc4_init( cntr )                                                                \
(                                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                              \
  CC4_STATIC_ASSERT(                                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                                    \
  ),                                                                                   \
  *(cntr) = (                                                                          \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? (CC4_TYPEOF_XP( *(cntr) ))&cc4_vec_placeholder  : \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? (CC4_TYPEOF_XP( *(cntr) ))&cc4_list_placeholder : \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? (CC4_TYPEOF_XP( *(cntr) ))&cc4_map_placeholder  : \
                          /* CC4_SET */ (CC4_TYPEOF_XP( *(cntr) ))&cc4_map_placeholder    \
  ),                                                                                   \
  (void)0                                                                              \
)                                                                                      \

#define cc4_size( cntr )                               \
(                                                     \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),             \
  CC4_STATIC_ASSERT(                                   \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||               \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||               \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||               \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                   \
  ),                                                  \
  /* Function select */                               \
  (                                                   \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_size  : \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_size : \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_size  : \
                          /* CC4_SET */ cc4_set_size    \
  )                                                   \
  /* Function args */                                 \
  (                                                   \
    *(cntr)                                           \
  )                                                   \
)                                                     \

#define cc4_cap( cntr )                              \
(                                                   \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),           \
  CC4_STATIC_ASSERT(                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC ||              \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP ||              \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                 \
  ),                                                \
  /* Function select */                             \
  (                                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_cap : \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_cap : \
                          /* CC4_SET */ cc4_set_cap   \
  )                                                 \
  /* Function args */                               \
  (                                                 \
    *(cntr)                                         \
  )                                                 \
)                                                   \

#define cc4_reserve( cntr, n )                                                                \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT(                                                                          \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                                          \
  ),                                                                                         \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_reserve :                                    \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_reserve :                                    \
                            /* CC4_SET */ cc4_set_reserve                                      \
    )                                                                                        \
    /* Function args */                                                                      \
    (                                                                                        \
      *(cntr),                                                                               \
      n,                                                                                     \
      CC4_EL_SIZE( *(cntr) ),                                                                 \
      CC4_LAYOUT( *(cntr) ),                                                                  \
      CC4_KEY_HASH( *(cntr) ),                                                                \
      CC4_KEY_LOAD( *(cntr) ),                                                                \
      CC4_REALLOC_FN,                                                                         \
      CC4_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_insert( ... ) CC4_SELECT_ON_NUM_ARGS( cc4_insert, __VA_ARGS__ )

#define cc4_insert_2( cntr, key )                                                             \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_SET ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_set_insert(                                                                           \
      *(cntr),                                                                               \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),                                     \
      true,                                                                                  \
      CC4_LAYOUT( *(cntr) ),                                                                  \
      CC4_KEY_HASH( *(cntr) ),                                                                \
      CC4_KEY_CMPR( *(cntr) ),                                                                \
      CC4_KEY_LOAD( *(cntr) ),                                                                \
      CC4_EL_DTOR( *(cntr) ),                                                                 \
      CC4_REALLOC_FN,                                                                         \
      CC4_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_insert_3( cntr, key, el )                                                         \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT(                                                                          \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP                                                          \
  ),                                                                                         \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_insert  :                                    \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_insert :                                    \
                            /* CC4_MAP */ cc4_map_insert                                       \
    )                                                                                        \
    /* Function args */                                                                      \
    (                                                                                        \
      *(cntr),                                                                               \
      &CC4_MAKE_LVAL_COPY( CC4_EL_TY( *(cntr) ), (el) ),                                       \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),                                     \
      true,                                                                                  \
      CC4_EL_SIZE( *(cntr) ),                                                                 \
      CC4_LAYOUT( *(cntr) ),                                                                  \
      CC4_KEY_HASH( *(cntr) ),                                                                \
      CC4_KEY_CMPR( *(cntr) ),                                                                \
      CC4_KEY_LOAD( *(cntr) ),                                                                \
      CC4_EL_DTOR( *(cntr) ),                                                                 \
      CC4_KEY_DTOR( *(cntr) ),                                                                \
      CC4_REALLOC_FN,                                                                         \
      CC4_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_insert_n( cntr, index, els, n )                                                   \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_VEC ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_vec_insert_n( *(cntr), (index), (els), (n), CC4_EL_SIZE( *(cntr) ), CC4_REALLOC_FN )    \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_push( cntr, el )                                                                  \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT(                                                                          \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST                                                         \
  ),                                                                                         \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    /* Function select */                                                                    \
    (                                                                                        \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ?  cc4_vec_push  :                                     \
                            /* CC4_LIST */ cc4_list_push                                       \
    )                                                                                        \
    /* Function args */                                                                      \
    (                                                                                        \
      *(cntr),                                                                               \
      &CC4_MAKE_LVAL_COPY( CC4_EL_TY( *(cntr) ), (el) ),                                       \
      CC4_EL_SIZE( *(cntr) ),                                                                 \
      CC4_REALLOC_FN                                                                          \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_push_n( cntr, els, n )                                                            \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_VEC ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_vec_push_n( *(cntr), (els), (n), CC4_EL_SIZE( *(cntr) ), CC4_REALLOC_FN )               \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_get_or_insert( ... ) CC4_SELECT_ON_NUM_ARGS( cc4_get_or_insert, __VA_ARGS__ )

#define cc4_get_or_insert_2( cntr, key )                                                      \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_SET ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_set_insert(                                                                           \
      *(cntr),                                                                               \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),                                     \
      false,                                                                                 \
      CC4_LAYOUT( *(cntr) ),                                                                  \
      CC4_KEY_HASH( *(cntr) ),                                                                \
      CC4_KEY_CMPR( *(cntr) ),                                                                \
      CC4_KEY_LOAD( *(cntr) ),                                                                \
      CC4_EL_DTOR( *(cntr) ),                                                                 \
      CC4_REALLOC_FN,                                                                         \
      CC4_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_get_or_insert_3( cntr, key, el )                                                  \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_MAP ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_map_insert(                                                                           \
      *(cntr),                                                                               \
      &CC4_MAKE_LVAL_COPY( CC4_EL_TY( *(cntr) ), (el) ),                                       \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),                                     \
      false,                                                                                 \
      CC4_EL_SIZE( *(cntr) ),                                                                 \
      CC4_LAYOUT( *(cntr) ),                                                                  \
      CC4_KEY_HASH( *(cntr) ),                                                                \
      CC4_KEY_CMPR( *(cntr) ),                                                                \
      CC4_KEY_LOAD( *(cntr) ),                                                                \
      CC4_EL_DTOR( *(cntr) ),                                                                 \
      CC4_KEY_DTOR( *(cntr) ),                                                                \
      CC4_REALLOC_FN,                                                                         \
      CC4_FREE_FN                                                                             \
    )                                                                                        \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( CC4_EL_TY( *(cntr) ) *, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                                            \

#define cc4_get( cntr, key )                                            \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC ||                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP ||                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_get :                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_get :                  \
                            /* CC4_SET */ cc4_set_get                    \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),               \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) ),                                            \
      CC4_KEY_HASH( *(cntr) ),                                          \
      CC4_KEY_CMPR( *(cntr) )                                           \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_key_for( cntr, itr )                                                              \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_MAP ),                                       \
  CC4_CAST_MAYBE_UNUSED(                                                                      \
    const CC4_KEY_TY( *(cntr) ) *,                                                            \
    cc4_map_key_for( (itr), CC4_EL_SIZE( *(cntr) ), CC4_LAYOUT( *(cntr) ) )                     \
  )                                                                                          \
)                                                                                            \

#define cc4_erase( cntr, key )                                           \
(                                                                       \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                               \
  CC4_STATIC_ASSERT(                                                     \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                 \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                     \
  ),                                                                    \
  CC4_IF_THEN_CAST_TY_1_ELSE_CAST_TY_2(                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP || CC4_CNTR_ID( *(cntr) ) == CC4_SET, \
    bool,                                                               \
    CC4_EL_TY( *(cntr) ) *,                                              \
    /* Function select */                                               \
    (                                                                   \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_erase  :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_erase :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_erase  :                \
                            /* CC4_SET */ cc4_set_erase                   \
    )                                                                   \
    /* Function args */                                                 \
    (                                                                   \
      *(cntr),                                                          \
      &CC4_MAKE_LVAL_COPY( CC4_KEY_TY( *(cntr) ), (key) ),                \
      CC4_EL_SIZE( *(cntr) ),                                            \
      CC4_LAYOUT( *(cntr) ),                                             \
      CC4_KEY_HASH( *(cntr) ),                                           \
      CC4_KEY_CMPR( *(cntr) ),                                           \
      CC4_EL_DTOR( *(cntr) ),                                            \
      CC4_KEY_DTOR( *(cntr) ),                                           \
      CC4_FREE_FN                                                        \
    )                                                                   \
  )                                                                     \
)                                                                       \

#define cc4_erase_n( cntr, index, n )                                                      \
(                                                                                         \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                 \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_VEC ),                                    \
  CC4_CAST_MAYBE_UNUSED(                                                                   \
    CC4_EL_TY( *(cntr) ) *,                                                                \
    cc4_vec_erase_n( *(cntr), (index), (n), CC4_EL_SIZE( *(cntr) ), CC4_EL_DTOR( *(cntr) ) ) \
  )                                                                                       \
)                                                                                         \

#define cc4_erase_itr( cntr, itr )                                    \
(                                                                    \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                            \
  CC4_STATIC_ASSERT(                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP ||                               \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                  \
  ),                                                                 \
  /* Function select */                                              \
  (                                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_erase_itr :            \
                          /* CC4_SET */ cc4_set_erase_itr              \
  )                                                                  \
  /* Function args */                                                \
  (                                                                  \
    *(cntr),                                                         \
    itr,                                                             \
    CC4_EL_SIZE( *(cntr) ),                                           \
    CC4_LAYOUT( *(cntr) ),                                            \
    CC4_EL_DTOR( *(cntr) ),                                           \
    CC4_KEY_DTOR( *(cntr) )                                           \
  )                                                                  \
)                                                                    \

#define cc4_splice( cntr, itr, src, src_itr )                                \
(                                                                           \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                   \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_LIST ),                     \
  CC4_STATIC_ASSERT( CC4_IS_SAME_TY( (cntr), (src) ) ),                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                      \
    *(cntr),                                                                \
    cc4_list_splice( *(cntr), itr, *(src), src_itr, CC4_REALLOC_FN )          \
  ),                                                                        \
  CC4_CAST_MAYBE_UNUSED( bool, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                           \

#define cc4_resize( cntr, n )                                                                 \
(                                                                                            \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                                    \
  CC4_STATIC_ASSERT( CC4_CNTR_ID( *(cntr) ) == CC4_VEC ),                                       \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                                       \
    *(cntr),                                                                                 \
    cc4_vec_resize( *(cntr), n, CC4_EL_SIZE( *(cntr) ), CC4_EL_DTOR( *(cntr) ), CC4_REALLOC_FN ) \
  ),                                                                                         \
  CC4_CAST_MAYBE_UNUSED( bool, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) )                  \
)                                                                                            \

#define cc4_shrink( cntr )                                                   \
(                                                                           \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                                   \
  CC4_STATIC_ASSERT(                                                         \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                     \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                     \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                     \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                         \
  ),                                                                        \
  CC4_POINT_HNDL_TO_ALLOCING_FN_RESULT(                                      \
    *(cntr),                                                                \
    /* Function select */                                                   \
    (                                                                       \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_shrink :                    \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_shrink :                    \
                            /* CC4_SET */ cc4_set_shrink                      \
    )                                                                       \
    /* Function args */                                                     \
    (                                                                       \
      *(cntr),                                                              \
      CC4_EL_SIZE( *(cntr) ),                                                \
      CC4_LAYOUT( *(cntr) ),      \
      CC4_KEY_HASH( *(cntr) ),                                               \
      CC4_KEY_LOAD( *(cntr) ),                                               \
      CC4_REALLOC_FN,                                                        \
      CC4_FREE_FN                                                            \
    )                                                                       \
  ),                                                                        \
  CC4_CAST_MAYBE_UNUSED( bool, CC4_FIX_HNDL_AND_RETURN_OTHER_PTR( *(cntr) ) ) \
)                                                                           \

#define cc4_init_clone( cntr, src )                                     \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_STATIC_ASSERT( CC4_IS_SAME_TY( *(cntr), *(src) ) ),                \
  CC4_CAST_MAYBE_UNUSED(                                                \
    bool,                                                              \
    *(cntr) = (CC4_TYPEOF_XP( *(cntr) ))                                \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_init_clone  :          \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_init_clone :          \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_init_clone  :          \
                            /* CC4_SET */ cc4_set_init_clone             \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(src),                                                          \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) ), \
      CC4_REALLOC_FN,                                                   \
      CC4_FREE_FN                                                       \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_clear( cntr )                                             \
(                                                                    \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                            \
  CC4_STATIC_ASSERT(                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                  \
  ),                                                                 \
  /* Function select */                                              \
  (                                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_clear  :               \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_clear :               \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_clear  :               \
                          /* CC4_SET */ cc4_set_clear                  \
  )                                                                  \
  /* Function args */                                                \
  (                                                                  \
    *(cntr),                                                         \
    CC4_EL_SIZE( *(cntr) ),                                           \
    CC4_LAYOUT( *(cntr) ), \
    CC4_EL_DTOR( *(cntr) ),                                           \
    CC4_KEY_DTOR( *(cntr) ),                                          \
    CC4_FREE_FN                                                       \
  )                                                                  \
)                                                                    \

#define cc4_cleanup( cntr )                                           \
(                                                                    \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                            \
  CC4_STATIC_ASSERT(                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                              \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                  \
  ),                                                                 \
  /* Function select */                                              \
  (                                                                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_cleanup  :             \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_cleanup :             \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_cleanup  :             \
                          /* CC4_SET */ cc4_set_cleanup                \
  )                                                                  \
  /* Function args */                                                \
  (                                                                  \
    *(cntr),                                                         \
    CC4_EL_SIZE( *(cntr) ),                                           \
    CC4_LAYOUT( *(cntr) ), \
    CC4_EL_DTOR( *(cntr) ),                                           \
    CC4_KEY_DTOR( *(cntr) ),                                          \
    CC4_FREE_FN                                                       \
  ),                                                                 \
  cc4_init( cntr )                                                    \
)                                                                    \

#define cc4_r_end( cntr )                                 \
(                                                        \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                \
  CC4_STATIC_ASSERT(                                      \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                  \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                      \
  ),                                                     \
  CC4_CAST_MAYBE_UNUSED(                                  \
    CC4_EL_TY( *(cntr) ) *,                               \
    /* Function select */                                \
    (                                                    \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_r_end : \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_r_end  : \
                            /* CC4_SET */ cc4_set_r_end    \
    )                                                    \
    /* Function args */                                  \
    (                                                    \
      *(cntr)                                            \
    )                                                    \
  )                                                      \
)                                                        \

#define cc4_end( cntr )                                                 \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_end  :                 \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_end :                 \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_end  :                 \
                            /* CC4_SET */ cc4_set_end                    \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) )  \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_first( cntr )                                               \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_first  :               \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_first :               \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_first  :               \
                            /* CC4_SET */ cc4_set_first                  \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      CC4_EL_SIZE( *(cntr) ), \
      CC4_LAYOUT( *(cntr) )  \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_last( cntr )                                                \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_last  :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_last :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_last  :                \
                            /* CC4_SET */ cc4_set_last                   \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) )  \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_next( cntr, itr )                                           \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_VEC  ? cc4_vec_next  :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_next :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_next  :                \
                            /* CC4_SET */ cc4_set_next                   \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      (itr),                                                           \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) )  \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_prev( cntr, itr )                                           \
(                                                                      \
  CC4_WARN_DUPLICATE_SIDE_EFFECTS( cntr ),                              \
  CC4_STATIC_ASSERT(                                                    \
    CC4_CNTR_ID( *(cntr) ) == CC4_LIST ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ||                                \
    CC4_CNTR_ID( *(cntr) ) == CC4_SET                                    \
  ),                                                                   \
  CC4_CAST_MAYBE_UNUSED(                                                \
    CC4_EL_TY( *(cntr) ) *,                                             \
    /* Function select */                                              \
    (                                                                  \
      CC4_CNTR_ID( *(cntr) ) == CC4_LIST ? cc4_list_prev :                \
      CC4_CNTR_ID( *(cntr) ) == CC4_MAP  ? cc4_map_prev  :                \
                            /* CC4_SET */ cc4_set_prev                   \
    )                                                                  \
    /* Function args */                                                \
    (                                                                  \
      *(cntr),                                                         \
      (itr),                                                           \
      CC4_EL_SIZE( *(cntr) ),                                           \
      CC4_LAYOUT( *(cntr) )  \
    )                                                                  \
  )                                                                    \
)                                                                      \

#define cc4_for_each( ... ) CC4_SELECT_ON_NUM_ARGS( cc4_for_each, __VA_ARGS__ )

#define cc4_for_each_2( cntr, i_name )                                                                                 \
  for( CC4_EL_TY( *(cntr) ) *i_name = cc4_first( cntr ); i_name != cc4_end( cntr ); i_name = cc4_next( (cntr), i_name ) ) \

#define cc4_for_each_3( cntr, key_ptr_name, i_name )                                                                   \
  for( CC4_EL_TY( *(cntr) ) *i_name = cc4_first( cntr ); i_name != cc4_end( cntr ); i_name = cc4_next( (cntr), i_name ) ) \
    for( const CC4_KEY_TY( *(cntr) ) *key_ptr_name = cc4_key_for( (cntr), i_name ); key_ptr_name; key_ptr_name = NULL ) \

#define cc4_r_for_each( ... ) CC4_SELECT_ON_NUM_ARGS( cc4_r_for_each, __VA_ARGS__ )

#define cc4_r_for_each_2( cntr, i_name )                                                                                \
  for( CC4_EL_TY( *(cntr) ) *i_name = cc4_last( cntr ); i_name != cc4_r_end( cntr ); i_name = cc4_prev( (cntr), i_name ) ) \

#define cc4_r_for_each_3( cntr, key_ptr_name, i_name )                                                                  \
  for( CC4_EL_TY( *(cntr) ) *i_name = cc4_last( cntr ); i_name != cc4_r_end( cntr ); i_name = cc4_prev( (cntr), i_name ) ) \
    for( const CC4_KEY_TY( *(cntr) ) *key_ptr_name = cc4_key_for( (cntr), i ); key_ptr_name; key_ptr_name = NULL )       \

/*--------------------------------------------------------------------------------------------------------------------*/
/*                         Destructor, comparison, and hash functions and custom load factors                         */
/*--------------------------------------------------------------------------------------------------------------------*/

// Octal counters that support up to 511 of each function type and 511 load factors.
#define CC4_N_DTORS_D1 0 // D1 = digit 1, i.e. least significant digit.
#define CC4_N_DTORS_D2 0
#define CC4_N_DTORS_D3 0
#define CC4_N_CMPRS_D1 0
#define CC4_N_CMPRS_D2 0
#define CC4_N_CMPRS_D3 0
#define CC4_N_HASHS_D1 0
#define CC4_N_HASHS_D2 0
#define CC4_N_HASHS_D3 0
#define CC4_N_LOADS_D1 0
#define CC4_N_LOADS_D2 0
#define CC4_N_LOADS_D3 0

#define CC4_CAT_3_( a, b, c ) a##b##c
#define CC4_CAT_3( a, b, c ) CC4_CAT_3_( a, b, c )
#define CC4_CAT_4_( a, b, c, d ) a##b##c##d
#define CC4_CAT_4( a, b, c, d ) CC4_CAT_4_( a, b, c, d )

// Macros that provide the current value of each counter as a three-digit octal number preceded by 0.
// These numbers are used to form unique type and function names to plug into CC4_EL_DTOR, CC4_KEY_DTOR, CC4_KEY_CMPR, etc.
#define CC4_N_DTORS CC4_CAT_4( 0, CC4_N_DTORS_D3, CC4_N_DTORS_D2, CC4_N_DTORS_D1 )
#define CC4_N_CMPRS CC4_CAT_4( 0, CC4_N_CMPRS_D3, CC4_N_CMPRS_D2, CC4_N_CMPRS_D1 )
#define CC4_N_HASHS CC4_CAT_4( 0, CC4_N_HASHS_D3, CC4_N_HASHS_D2, CC4_N_HASHS_D1 )
#define CC4_N_LOADS CC4_CAT_4( 0, CC4_N_LOADS_D3, CC4_N_LOADS_D2, CC4_N_LOADS_D1 )

// CC4_FOR_EACH_XXX macros that call macro m with the first argument n, where n = [0, counter XXX ),
// and the second argument arg.
// These are used to construct _Generic or ternary expressions that select the correct function (or load factor) for a
// container's key or element type (see below).

#define CC4_CALL_MACRO( m, arg, n ) m( n, arg )
#define CC4_R1_0( m, arg, d3, d2 )
#define CC4_R1_1( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 0 ) )
#define CC4_R1_2( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 1 ) ) CC4_R1_1( m, arg, d3, d2 )
#define CC4_R1_3( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 2 ) ) CC4_R1_2( m, arg, d3, d2 )
#define CC4_R1_4( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 3 ) ) CC4_R1_3( m, arg, d3, d2 )
#define CC4_R1_5( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 4 ) ) CC4_R1_4( m, arg, d3, d2 )
#define CC4_R1_6( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 5 ) ) CC4_R1_5( m, arg, d3, d2 )
#define CC4_R1_7( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 6 ) ) CC4_R1_6( m, arg, d3, d2 )
#define CC4_R1_8( m, arg, d3, d2 ) CC4_CALL_MACRO( m, arg, CC4_CAT_4( 0, d3, d2, 7 ) ) CC4_R1_7( m, arg, d3, d2 )
#define CC4_R2_0( m, arg, d3 )
#define CC4_R2_1( m, arg, d3 ) CC4_R1_8( m, arg, d3, 0 )
#define CC4_R2_2( m, arg, d3 ) CC4_R1_8( m, arg, d3, 1 ) CC4_R2_1( m, arg, d3 )
#define CC4_R2_3( m, arg, d3 ) CC4_R1_8( m, arg, d3, 2 ) CC4_R2_2( m, arg, d3 )
#define CC4_R2_4( m, arg, d3 ) CC4_R1_8( m, arg, d3, 3 ) CC4_R2_3( m, arg, d3 )
#define CC4_R2_5( m, arg, d3 ) CC4_R1_8( m, arg, d3, 4 ) CC4_R2_4( m, arg, d3 )
#define CC4_R2_6( m, arg, d3 ) CC4_R1_8( m, arg, d3, 5 ) CC4_R2_5( m, arg, d3 )
#define CC4_R2_7( m, arg, d3 ) CC4_R1_8( m, arg, d3, 6 ) CC4_R2_6( m, arg, d3 )
#define CC4_R2_8( m, arg, d3 ) CC4_R1_8( m, arg, d3, 7 ) CC4_R2_7( m, arg, d3 )
#define CC4_R3_0( m, arg )
#define CC4_R3_1( m, arg ) CC4_R2_8( m, arg, 0 )
#define CC4_R3_2( m, arg ) CC4_R2_8( m, arg, 1 ) CC4_R3_1( m, arg )
#define CC4_R3_3( m, arg ) CC4_R2_8( m, arg, 2 ) CC4_R3_2( m, arg )
#define CC4_R3_4( m, arg ) CC4_R2_8( m, arg, 3 ) CC4_R3_3( m, arg )
#define CC4_R3_5( m, arg ) CC4_R2_8( m, arg, 4 ) CC4_R3_4( m, arg )
#define CC4_R3_6( m, arg ) CC4_R2_8( m, arg, 5 ) CC4_R3_5( m, arg )
#define CC4_R3_7( m, arg ) CC4_R2_8( m, arg, 6 ) CC4_R3_6( m, arg )

#define CC4_FOR_OCT_COUNT( m, arg, d3, d2, d1 ) \
CC4_CAT_2( CC4_R1_, d1 )( m, arg, d3, d2 )       \
CC4_CAT_2( CC4_R2_, d2 )( m, arg, d3 )           \
CC4_CAT_2( CC4_R3_, d3 )( m, arg )               \

#define CC4_FOR_EACH_DTOR( m, arg ) CC4_FOR_OCT_COUNT( m, arg, CC4_N_DTORS_D3, CC4_N_DTORS_D2, CC4_N_DTORS_D1 )
#define CC4_FOR_EACH_CMPR( m, arg ) CC4_FOR_OCT_COUNT( m, arg, CC4_N_CMPRS_D3, CC4_N_CMPRS_D2, CC4_N_CMPRS_D1 )
#define CC4_FOR_EACH_HASH( m, arg ) CC4_FOR_OCT_COUNT( m, arg, CC4_N_HASHS_D3, CC4_N_HASHS_D2, CC4_N_HASHS_D1 )
#define CC4_FOR_EACH_LOAD( m, arg ) CC4_FOR_OCT_COUNT( m, arg, CC4_N_LOADS_D3, CC4_N_LOADS_D2, CC4_N_LOADS_D1 )

// Macros for inferring the destructor, comparison, or hash function or load factor associated with a container's
// key or element type, as well as for determining whether a comparison or hash function exists for a type and inferring
// certain map function arguments in bulk (argument packs) from they key type.
// In C, we use the CC4_FOR_EACH_XXXX macros above to create _Generic expressions that select the correct user-defined
// function or load factor for the container's key or element types.
// For comparison and hash functions, the list of user-defined functions is followed by a nested _Generic statement
// containing the default functions for integer types and NULL-terminated strings, which allows the user to overwrite
// the default functions.
// In C++, we use the same macros combined with std::is_same and ternary expressions to emulate _Generic statements.
// Unfortunately, the behavior is not identical because the C++ versions won't generate compile errors if the user
// defines multiple functions (e.g. multiple destructors) for the same type.
// Hence, it is up to the user to make sure they are not doing that if they are compiling for C++.

#ifdef __cplusplus

#define CC4_EL_DTOR_SLOT( n, arg ) std::is_same<arg, cc4_dtor_##n##_ty>::value ? cc4_dtor_##n##_fn :
#define CC4_EL_DTOR( cntr )                              \
(                                                       \
  CC4_FOR_EACH_DTOR( CC4_EL_DTOR_SLOT, CC4_EL_TY( cntr ) ) \
  (void (*)( void * ))NULL                              \
)                                                       \

#define CC4_KEY_DTOR_SLOT( n, arg )                           \
std::is_same<                                                \
  CC4_TYPEOF_XP(**arg),                                       \
  CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( arg ), cc4_dtor_##n##_ty ) \
>::value ? cc4_dtor_##n##_fn :                                \

#define CC4_KEY_DTOR( cntr )                  \
(                                            \
  CC4_FOR_EACH_DTOR( CC4_KEY_DTOR_SLOT, cntr ) \
  (void (*)( void * ))NULL                   \
)                                            \

#define CC4_KEY_CMPR_SLOT( n, arg )                                                                      \
std::is_same<CC4_TYPEOF_XP(**arg), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( arg ), cc4_cmpr_##n##_ty ) >::value ? \
  cc4_cmpr_##n##_fn                                                                                    : \

#define CC4_KEY_CMPR( cntr )                                                                                  \
(                                                                                                            \
  CC4_FOR_EACH_CMPR( CC4_KEY_CMPR_SLOT, cntr )                                                                 \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char )>::value               ? \
    cc4_cmpr_char                                                                                           : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char )>::value      ? \
    cc4_cmpr_unsigned_char                                                                                  : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char )>::value        ? \
    cc4_cmpr_signed_char                                                                                    : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short )>::value     ? \
    cc4_cmpr_unsigned_short                                                                                 : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short )>::value              ? \
    cc4_cmpr_short                                                                                          : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int )>::value       ? \
    cc4_cmpr_unsigned_int                                                                                   : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int )>::value                ? \
    cc4_cmpr_int                                                                                            : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long )>::value      ? \
    cc4_cmpr_unsigned_long                                                                                  : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long )>::value               ? \
    cc4_cmpr_long                                                                                           : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long )>::value ? \
    cc4_cmpr_unsigned_long_long                                                                             : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long )>::value          ? \
    cc4_cmpr_long_long                                                                                      : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), size_t )>::value             ? \
    cc4_cmpr_size_t                                                                                         : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * )>::value             ? \
    cc4_cmpr_c_string                                                                                       : \
  (int (*)( void *, void * ))NULL                                                                            \
)                                                                                                            \

#define CC4_KEY_HASH_SLOT( n, arg )                           \
std::is_same<                                                \
  CC4_TYPEOF_XP(**arg),                                       \
  CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( arg ), cc4_hash_##n##_ty ) \
>::value ? cc4_hash_##n##_fn :                                \

#define CC4_KEY_HASH( cntr )                                                                                  \
(                                                                                                            \
  CC4_FOR_EACH_HASH( CC4_KEY_HASH_SLOT, cntr )                                                                 \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char )>::value               ? \
    cc4_hash_char                                                                                           : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char )>::value      ? \
    cc4_hash_unsigned_char                                                                                  : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char )>::value        ? \
    cc4_hash_signed_char                                                                                    : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short )>::value     ? \
    cc4_hash_unsigned_short                                                                                 : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short )>::value              ? \
    cc4_hash_short                                                                                          : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int )>::value       ? \
    cc4_hash_unsigned_int                                                                                   : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int )>::value                ? \
    cc4_hash_int                                                                                            : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long )>::value      ? \
    cc4_hash_unsigned_long                                                                                  : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long )>::value               ? \
    cc4_hash_long                                                                                           : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long )>::value ? \
    cc4_hash_unsigned_long_long                                                                             : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long )>::value          ? \
    cc4_hash_long_long                                                                                      : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), size_t )>::value             ? \
    cc4_hash_size_t                                                                                         : \
  std::is_same<CC4_TYPEOF_XP(**cntr), CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * )>::value             ? \
    cc4_hash_c_string                                                                                       : \
  (size_t (*)( void * ))NULL                                                                                 \
)                                                                                                            \

#define CC4_HAS_CMPR_SLOT( n, arg ) std::is_same<arg, cc4_cmpr_##n##_ty>::value ? true :
#define CC4_HAS_CMPR( ty )                              \
(                                                      \
  std::is_same<ty, char>::value               ? true : \
  std::is_same<ty, unsigned char>::value      ? true : \
  std::is_same<ty, signed char>::value        ? true : \
  std::is_same<ty, unsigned short>::value     ? true : \
  std::is_same<ty, signed short>::value       ? true : \
  std::is_same<ty, unsigned int>::value       ? true : \
  std::is_same<ty, signed int>::value         ? true : \
  std::is_same<ty, unsigned long>::value      ? true : \
  std::is_same<ty, signed long>::value        ? true : \
  std::is_same<ty, unsigned long long>::value ? true : \
  std::is_same<ty, signed long long>::value   ? true : \
  std::is_same<ty, size_t>::value             ? true : \
  std::is_same<ty, char *>::value             ? true : \
  CC4_FOR_EACH_CMPR( CC4_HAS_CMPR_SLOT, ty )             \
  false                                                \
)                                                      \

#define CC4_HAS_HASH_SLOT( n, arg ) std::is_same<arg, cc4_hash_##n##_ty>::value ? true :
#define CC4_HAS_HASH( ty )                              \
(                                                      \
  std::is_same<ty, char>::value               ? true : \
  std::is_same<ty, unsigned char>::value      ? true : \
  std::is_same<ty, signed char>::value        ? true : \
  std::is_same<ty, unsigned short>::value     ? true : \
  std::is_same<ty, signed short>::value       ? true : \
  std::is_same<ty, unsigned int>::value       ? true : \
  std::is_same<ty, signed int>::value         ? true : \
  std::is_same<ty, unsigned long>::value      ? true : \
  std::is_same<ty, signed long>::value        ? true : \
  std::is_same<ty, unsigned long long>::value ? true : \
  std::is_same<ty, signed long long>::value   ? true : \
  std::is_same<ty, size_t>::value             ? true : \
  std::is_same<ty, char *>::value             ? true : \
  CC4_FOR_EACH_HASH( CC4_HAS_HASH_SLOT, ty )             \
  false                                                \
)                                                      \

#define CC4_KEY_LOAD_SLOT( n, arg )                           \
std::is_same<                                                \
  CC4_TYPEOF_XP(**arg),                                       \
  CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( arg ), cc4_load_##n##_ty ) \
>::value ? cc4_load_##n##_val :                               \

#define CC4_KEY_LOAD( cntr )                  \
(                                            \
  CC4_FOR_EACH_LOAD( CC4_KEY_LOAD_SLOT, cntr ) \
  CC4_DEFAULT_LOAD                            \
)                                            \

#define CC4_LAYOUT( cntr )                                                        \
cc4_layout(                                                                       \
  CC4_CNTR_ID( cntr ),                                                            \
  CC4_EL_SIZE( cntr ),                                                            \
  alignof( CC4_EL_TY( cntr ) ),                                                   \
  cc4_key_details_ty{ sizeof( CC4_KEY_TY( cntr ) ), alignof( CC4_KEY_TY( cntr ) ) } \
)                                                                                \

#else

#define CC4_EL_DTOR_SLOT( n, arg ) cc4_dtor_##n##_ty: cc4_dtor_##n##_fn,
#define CC4_EL_DTOR( cntr )             \
_Generic( (CC4_EL_TY( cntr )){ 0 },     \
  CC4_FOR_EACH_DTOR( CC4_EL_DTOR_SLOT, ) \
  default: (cc4_dtor_fnptr_ty)NULL      \
)                                      \

#define CC4_KEY_DTOR_SLOT( n, arg ) CC4_MAKE_BASE_FNPTR_TY( arg, cc4_dtor_##n##_ty ): cc4_dtor_##n##_fn,
#define CC4_KEY_DTOR( cntr )                              \
_Generic( (**cntr),                                      \
  CC4_FOR_EACH_DTOR( CC4_KEY_DTOR_SLOT, CC4_EL_TY( cntr ) ) \
  default: (cc4_dtor_fnptr_ty)NULL                        \
)                                                        \

#define CC4_KEY_CMPR_SLOT( n, arg ) CC4_MAKE_BASE_FNPTR_TY( arg, cc4_cmpr_##n##_ty ): cc4_cmpr_##n##_fn,
#define CC4_KEY_CMPR( cntr )                                                                    \
_Generic( (**cntr),                                                                            \
  CC4_FOR_EACH_CMPR( CC4_KEY_CMPR_SLOT, CC4_EL_TY( cntr ) )                                       \
  default: _Generic( (**cntr),                                                                 \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char ):               cc4_cmpr_char,               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char ):      cc4_cmpr_unsigned_char,      \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char ):        cc4_cmpr_signed_char,        \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short ):     cc4_cmpr_unsigned_short,     \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short ):              cc4_cmpr_short,              \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int ):       cc4_cmpr_unsigned_int,       \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int ):                cc4_cmpr_int,                \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long ):      cc4_cmpr_unsigned_long,      \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long ):               cc4_cmpr_long,               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long ): cc4_cmpr_unsigned_long_long, \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long ):          cc4_cmpr_long_long,          \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), cc4_maybe_size_t ):    cc4_cmpr_size_t,             \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * ):             cc4_cmpr_c_string,           \
    default: (cc4_cmpr_fnptr_ty)NULL                                                            \
  )                                                                                            \
)                                                                                              \

#define CC4_KEY_HASH_SLOT( n, arg ) CC4_MAKE_BASE_FNPTR_TY( arg, cc4_hash_##n##_ty ): cc4_hash_##n##_fn,
#define CC4_KEY_HASH( cntr )                                                                    \
_Generic( (**cntr),                                                                            \
  CC4_FOR_EACH_HASH( CC4_KEY_HASH_SLOT, CC4_EL_TY( cntr ) )                                       \
  default: _Generic( (**cntr),                                                                 \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char ):               cc4_hash_char,               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char ):      cc4_hash_unsigned_char,      \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char ):        cc4_hash_signed_char,        \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short ):     cc4_hash_unsigned_short,     \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short ):              cc4_hash_short,              \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int ):       cc4_hash_unsigned_int,       \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int ):                cc4_hash_int,                \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long ):      cc4_hash_unsigned_long,      \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long ):               cc4_hash_long,               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long ): cc4_hash_unsigned_long_long, \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long ):          cc4_hash_long_long,          \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), cc4_maybe_size_t ):    cc4_hash_size_t,             \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * ):             cc4_hash_c_string,           \
    default: (cc4_hash_fnptr_ty)NULL                                                            \
  )                                                                                            \
)                                                                                              \

#define CC4_HAS_CMPR_SLOT( n, arg ) cc4_cmpr_##n##_ty: true,
#define CC4_HAS_CMPR( ty )               \
_Generic( (ty){ 0 },                    \
  CC4_FOR_EACH_CMPR( CC4_HAS_CMPR_SLOT, ) \
  default: _Generic( (ty){ 0 },         \
    char:               true,           \
    unsigned char:      true,           \
    signed char:        true,           \
    unsigned short:     true,           \
    short:              true,           \
    unsigned int:       true,           \
    int:                true,           \
    unsigned long:      true,           \
    long:               true,           \
    unsigned long long: true,           \
    long long:          true,           \
    cc4_maybe_size_t:    true,           \
    char *:             true,           \
    default:            false           \
  )                                     \
)                                       \

#define CC4_HAS_HASH_SLOT( n, arg ) cc4_hash_##n##_ty: true,
#define CC4_HAS_HASH( ty )               \
_Generic( (ty){ 0 },                    \
  CC4_FOR_EACH_HASH( CC4_HAS_HASH_SLOT, ) \
  default: _Generic( (ty){ 0 },         \
    char:               true,           \
    unsigned char:      true,           \
    signed char:        true,           \
    unsigned short:     true,           \
    short:              true,           \
    unsigned int:       true,           \
    int:                true,           \
    unsigned long:      true,           \
    long:               true,           \
    unsigned long long: true,           \
    long long:          true,           \
    cc4_maybe_size_t:    true,           \
    char *:             true,           \
    default:            false           \
  )                                     \
)                                       \

#define CC4_KEY_LOAD_SLOT( n, arg ) CC4_MAKE_BASE_FNPTR_TY( arg, cc4_load_##n##_ty ): cc4_load_##n##_val,
#define CC4_KEY_LOAD( cntr )                              \
_Generic( (**cntr),                                      \
  CC4_FOR_EACH_LOAD( CC4_KEY_LOAD_SLOT, CC4_EL_TY( cntr ) ) \
  default: CC4_DEFAULT_LOAD                               \
)                                                        \

#define CC4_KEY_DETAILS_SLOT( n, arg )                                               \
CC4_MAKE_BASE_FNPTR_TY( arg, cc4_cmpr_##n##_ty ):                                     \
  ( cc4_key_details_ty ){ sizeof( cc4_cmpr_##n##_ty ), alignof( cc4_cmpr_##n##_ty ) }, \

#define CC4_KEY_DETAILS( cntr )                                                              \
_Generic( (**cntr),                                                                         \
  CC4_FOR_EACH_CMPR( CC4_KEY_DETAILS_SLOT, CC4_EL_TY( cntr ) )                                 \
  default: _Generic( (**cntr),                                                              \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char ):                                        \
      ( cc4_key_details_ty ){ sizeof( char ), alignof( char ) },                             \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned char ) :                              \
      ( cc4_key_details_ty ){ sizeof( unsigned char ), alignof( unsigned char ) },           \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), signed char ) :                                \
      ( cc4_key_details_ty ){ sizeof( signed char ), alignof( signed char ) },               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned short ) :                             \
      ( cc4_key_details_ty ){ sizeof( unsigned short ), alignof( unsigned short ) },         \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), short ) :                                      \
      ( cc4_key_details_ty ){ sizeof( short ), alignof( short ) },                           \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned int ) :                               \
      ( cc4_key_details_ty ){ sizeof( unsigned int ), alignof( unsigned int ) },             \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), int ) :                                        \
      ( cc4_key_details_ty ){ sizeof( int ), alignof( int ) },                               \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long ):                               \
      ( cc4_key_details_ty ){ sizeof( unsigned long ), alignof( unsigned long ) },           \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long ):                                        \
      ( cc4_key_details_ty ){ sizeof( long ), alignof( long ) },                             \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), unsigned long long ):                          \
      ( cc4_key_details_ty ){ sizeof( unsigned long long ), alignof( unsigned long long ) }, \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), long long ):                                   \
      ( cc4_key_details_ty ){ sizeof( long long ), alignof( long long ) },                   \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), cc4_maybe_size_t ):                             \
      ( cc4_key_details_ty ){ sizeof( cc4_maybe_size_t ), alignof( cc4_maybe_size_t ) },       \
    CC4_MAKE_BASE_FNPTR_TY( CC4_EL_TY( cntr ), char * ):                                      \
      ( cc4_key_details_ty ){ sizeof( char * ), alignof( char * ) },                         \
    default: ( cc4_key_details_ty ){ 0 }                                                     \
  )                                                                                         \
)                                                                                           \

#define CC4_LAYOUT( cntr )                                                                                \
cc4_layout( CC4_CNTR_ID( cntr ), CC4_EL_SIZE( cntr ), alignof( CC4_EL_TY( cntr ) ), CC4_KEY_DETAILS( cntr ) ) \

#endif

// Macros for extracting the type and function body or load factor from user-defined DTOR, CMPR, HASH, and LOAD macros. 
#define CC4_1ST_ARG_( _1, ... )    _1
#define CC4_1ST_ARG( ... )         CC4_1ST_ARG_( __VA_ARGS__ )
#define CC4_OTHER_ARGS_( _1, ... ) __VA_ARGS__
#define CC4_OTHER_ARGS( ... )      CC4_OTHER_ARGS_( __VA_ARGS__ )

// Default hash and comparison functions for fundamental types.

// Integer types.

// TODO: Unified max_int_t hash function

static inline int cc4_cmpr_char( void *void_val_1, void *void_val_2 )
{
  return ( *(char *)void_val_1 > *(char *)void_val_2 ) - ( *(char *)void_val_1 < *(char *)void_val_2 );
}

static inline size_t cc4_hash_char( void *void_val )
{
  return *(char *)void_val;
}

static inline int cc4_cmpr_unsigned_char( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned char *)void_val_1 > *(unsigned char *)void_val_2 ) -
         ( *(unsigned char *)void_val_1 < *(unsigned char *)void_val_2 );
}

static inline size_t cc4_hash_unsigned_char( void *void_val )
{
  return *(unsigned char *)void_val;
}

static inline int cc4_cmpr_signed_char( void *void_val_1, void *void_val_2 )
{
  return ( *(signed char *)void_val_1 > *(signed char *)void_val_2 ) -
         ( *(signed char *)void_val_1 < *(signed char *)void_val_2 );
}

static inline size_t cc4_hash_signed_char( void *void_val )
{
  return *(signed char *)void_val;
}

static inline int cc4_cmpr_unsigned_short( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned short *)void_val_1 > *(unsigned short *)void_val_2 ) -
         ( *(unsigned short *)void_val_1 < *(unsigned short *)void_val_2 );
}

static inline size_t cc4_hash_unsigned_short( void *void_val )
{
  return *(unsigned short *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_short( void *void_val_1, void *void_val_2 )
{
  return ( *(short *)void_val_1 > *(short *)void_val_2 ) - ( *(short *)void_val_1 < *(short *)void_val_2 );
}

static inline size_t cc4_hash_short( void *void_val )
{
  return *(short *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_unsigned_int( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned int *)void_val_1 > *(unsigned int *)void_val_2 ) -
         ( *(unsigned int *)void_val_1 < *(unsigned int *)void_val_2 );
}

static inline size_t cc4_hash_unsigned_int( void *void_val )
{
  return *(unsigned int *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_int( void *void_val_1, void *void_val_2 )
{
  return ( *(int *)void_val_1 > *(int *)void_val_2 ) - ( *(int *)void_val_1 < *(int *)void_val_2 );
}

static inline size_t cc4_hash_int( void *void_val )
{
  return *(int *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_unsigned_long( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned long *)void_val_1 > *(unsigned long *)void_val_2 ) -
         ( *(unsigned long *)void_val_1 < *(unsigned long *)void_val_2 );
}

static inline size_t cc4_hash_unsigned_long( void *void_val )
{
  return *(unsigned long *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_long( void *void_val_1, void *void_val_2 )
{
  return ( *(long *)void_val_1 > *(long *)void_val_2 ) - ( *(long *)void_val_1 < *(long *)void_val_2 );
}

static inline size_t cc4_hash_long( void *void_val )
{
  return *(long *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_unsigned_long_long( void *void_val_1, void *void_val_2 )
{
  return ( *(unsigned long long *)void_val_1 > *(unsigned long long *)void_val_2 ) -
         ( *(unsigned long long *)void_val_1 < *(unsigned long long *)void_val_2 );
}

static inline size_t cc4_hash_unsigned_long_long( void *void_val )
{
  return *(unsigned long long *)void_val * 2654435761ull;
}

static inline int cc4_cmpr_long_long( void *void_val_1, void *void_val_2 )
{
  return ( *(long long *)void_val_1 > *(long long *)void_val_2 ) - 
         ( *(long long *)void_val_1 < *(long long *)void_val_2 );
}

static inline size_t cc4_hash_long_long( void *void_val )
{
  return *(long long *)void_val * 2654435761ull;
}

// size_t could be an alias for a fundamental integer type or a distinct type.
// Hence, in C we have to handle it as a special case so that it doesn't clash with another type in _Generic statements.
// If size_t is an alias, cc4_maybe_size_t will be a dummy type used in no other context.
// Otherwise, cc4_maybe_size_t will be an alias for size_t.

#ifndef __cplusplus

typedef struct { char nothing; } cc4_size_t_dummy;

typedef CC4_TYPEOF_XP(
  _Generic( (size_t){ 0 },
    unsigned short:     (cc4_size_t_dummy){ 0 },
    short:              (cc4_size_t_dummy){ 0 },
    unsigned int:       (cc4_size_t_dummy){ 0 },
    int:                (cc4_size_t_dummy){ 0 },
    unsigned long:      (cc4_size_t_dummy){ 0 },
    long:               (cc4_size_t_dummy){ 0 },
    unsigned long long: (cc4_size_t_dummy){ 0 },
    long long:          (cc4_size_t_dummy){ 0 },
    default:            (size_t){ 0 }
  )
) cc4_maybe_size_t;

#endif

static inline int cc4_cmpr_size_t( void *void_val_1, void *void_val_2 )
{
  return ( *(size_t *)void_val_1 > *(size_t *)void_val_2 ) - ( *(size_t *)void_val_1 < *(size_t *)void_val_2 );
}

static inline size_t cc4_hash_size_t( void *void_val )
{
  return *(size_t *)void_val * 2654435761ull;
}

// Null-terminated C strings.
// We use FNV-1a because newer, faster alternatives that process word-sized chunks require prior knowledge of the
// string's length.

static inline int cc4_cmpr_c_string( void *void_val_1, void *void_val_2 )
{
  return strcmp( *(char **)void_val_1, *(char **)void_val_2 );
}

#if SIZE_MAX == 0xFFFFFFFF // 32-bit size_t.

static inline size_t cc4_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0x01000193;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x811c9dc5;

    return hash;
}

#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF // 64-bit size_t.

static inline size_t cc4_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0xcbf29ce484222325;
    while( *val )
      hash = ( (unsigned char)*val++ ^ hash ) * 0x100000001b3;

    return hash;
}

#else // Strange size_t.

static inline size_t cc4_hash_c_string( void *void_val )
{
    char *val = *(char **)void_val;
    size_t hash = 0;
    while( *val )
        hash = hash * 131 + (unsigned char)*val++;

    return hash;
}

#endif

#endif

#else/*---------------------------------------------------------------------------------------------------------------*/
/*                                                                                                                    */
/*                       DEFINING DESTRUCTOR, COMPARISON, OR HASH FUNCTION OR LOAD FACTOR MODE                        */
/*                                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef CC4_DTOR

// Convert the user-defined CC4_DTOR macro into a cc4_dtor_XXXX_ty and cc4_dtor_XXXX_fn pair that can be pluged into the
// CC4_GET_EL_DTOR and CC4_GET_KEY_DTOR macros above.

typedef CC4_TYPEOF_TY( CC4_1ST_ARG( CC4_DTOR ) ) CC4_CAT_3( cc4_dtor_, CC4_N_DTORS, _ty );

static inline void CC4_CAT_3( cc4_dtor_, CC4_N_DTORS, _fn )( void *void_val )
{
  CC4_CAT_3( cc4_dtor_, CC4_N_DTORS, _ty ) val = *(CC4_CAT_3( cc4_dtor_, CC4_N_DTORS, _ty ) *)void_val;
  CC4_OTHER_ARGS( CC4_DTOR )
}

// Increment DTOR counter.
#if CC4_N_DTORS_D1 == 0
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 1
#elif CC4_N_DTORS_D1 == 1
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 2
#elif CC4_N_DTORS_D1 == 2
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 3
#elif CC4_N_DTORS_D1 == 3
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 4
#elif CC4_N_DTORS_D1 == 4
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 5
#elif CC4_N_DTORS_D1 == 5
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 6
#elif CC4_N_DTORS_D1 == 6
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 7
#elif CC4_N_DTORS_D1 == 7
#undef CC4_N_DTORS_D1
#define CC4_N_DTORS_D1 0
#if CC4_N_DTORS_D2 == 0
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 1
#elif CC4_N_DTORS_D2 == 1
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 2
#elif CC4_N_DTORS_D2 == 2
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 3
#elif CC4_N_DTORS_D2 == 3
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 4
#elif CC4_N_DTORS_D2 == 4
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 5
#elif CC4_N_DTORS_D2 == 5
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 6
#elif CC4_N_DTORS_D2 == 6
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 7
#elif CC4_N_DTORS_D2 == 7
#undef CC4_N_DTORS_D2
#define CC4_N_DTORS_D2 0
#if CC4_N_DTORS_D3 == 0
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 1
#elif CC4_N_DTORS_D3 == 1
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 2
#elif CC4_N_DTORS_D3 == 2
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 3
#elif CC4_N_DTORS_D3 == 3
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 4
#elif CC4_N_DTORS_D3 == 4
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 5
#elif CC4_N_DTORS_D3 == 5
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 6
#elif CC4_N_DTORS_D3 == 6
#undef CC4_N_DTORS_D3
#define CC4_N_DTORS_D3 7
#elif CC4_N_DTORS_D3 == 7
#error Sorry, number of destructor functions is limited to 511.
#endif
#endif
#endif

#undef CC4_DTOR
#endif

#ifdef CC4_CMPR

typedef CC4_TYPEOF_TY( CC4_1ST_ARG( CC4_CMPR ) ) CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _ty );

static inline int CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _fn )( void *void_val_1, void *void_val_2 )
{
  CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _ty ) val_1 = *(CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _ty ) *)void_val_1;
  CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _ty ) val_2 = *(CC4_CAT_3( cc4_cmpr_, CC4_N_CMPRS, _ty ) *)void_val_2;
  CC4_OTHER_ARGS( CC4_CMPR )
}

#if CC4_N_CMPRS_D1 == 0
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 1
#elif CC4_N_CMPRS_D1 == 1
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 2
#elif CC4_N_CMPRS_D1 == 2
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 3
#elif CC4_N_CMPRS_D1 == 3
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 4
#elif CC4_N_CMPRS_D1 == 4
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 5
#elif CC4_N_CMPRS_D1 == 5
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 6
#elif CC4_N_CMPRS_D1 == 6
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 7
#elif CC4_N_CMPRS_D1 == 7
#undef CC4_N_CMPRS_D1
#define CC4_N_CMPRS_D1 0
#if CC4_N_CMPRS_D2 == 0
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 1
#elif CC4_N_CMPRS_D2 == 1
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 2
#elif CC4_N_CMPRS_D2 == 2
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 3
#elif CC4_N_CMPRS_D2 == 3
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 4
#elif CC4_N_CMPRS_D2 == 4
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 5
#elif CC4_N_CMPRS_D2 == 5
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 6
#elif CC4_N_CMPRS_D2 == 6
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 7
#elif CC4_N_CMPRS_D2 == 7
#undef CC4_N_CMPRS_D2
#define CC4_N_CMPRS_D2 0
#if CC4_N_CMPRS_D3 == 0
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 1
#elif CC4_N_CMPRS_D3 == 1
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 2
#elif CC4_N_CMPRS_D3 == 2
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 3
#elif CC4_N_CMPRS_D3 == 3
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 4
#elif CC4_N_CMPRS_D3 == 4
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 5
#elif CC4_N_CMPRS_D3 == 5
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 6
#elif CC4_N_CMPRS_D3 == 6
#undef CC4_N_CMPRS_D3
#define CC4_N_CMPRS_D3 7
#elif CC4_N_CMPRS_D3 == 7
#error Sorry, number of comparison functions is limited to 511.
#endif
#endif
#endif

#undef CC4_CMPR
#endif

#ifdef CC4_HASH

typedef CC4_TYPEOF_TY( CC4_1ST_ARG( CC4_HASH ) ) CC4_CAT_3( cc4_hash_, CC4_N_HASHS, _ty );

static inline size_t CC4_CAT_3( cc4_hash_, CC4_N_HASHS, _fn )( void *void_val )
{
  CC4_CAT_3( cc4_hash_, CC4_N_HASHS, _ty ) val = *(CC4_CAT_3( cc4_hash_, CC4_N_HASHS, _ty ) *)void_val;
  CC4_OTHER_ARGS( CC4_HASH )
}

#if CC4_N_HASHS_D1 == 0
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 1
#elif CC4_N_HASHS_D1 == 1
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 2
#elif CC4_N_HASHS_D1 == 2
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 3
#elif CC4_N_HASHS_D1 == 3
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 4
#elif CC4_N_HASHS_D1 == 4
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 5
#elif CC4_N_HASHS_D1 == 5
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 6
#elif CC4_N_HASHS_D1 == 6
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 7
#elif CC4_N_HASHS_D1 == 7
#undef CC4_N_HASHS_D1
#define CC4_N_HASHS_D1 0
#if CC4_N_HASHS_D2 == 0
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 1
#elif CC4_N_HASHS_D2 == 1
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 2
#elif CC4_N_HASHS_D2 == 2
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 3
#elif CC4_N_HASHS_D2 == 3
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 4
#elif CC4_N_HASHS_D2 == 4
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 5
#elif CC4_N_HASHS_D2 == 5
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 6
#elif CC4_N_HASHS_D2 == 6
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 7
#elif CC4_N_HASHS_D2 == 7
#undef CC4_N_HASHS_D2
#define CC4_N_HASHS_D2 0
#if CC4_N_HASHS_D3 == 0
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 1
#elif CC4_N_HASHS_D3 == 1
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 2
#elif CC4_N_HASHS_D3 == 2
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 3
#elif CC4_N_HASHS_D3 == 3
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 4
#elif CC4_N_HASHS_D3 == 4
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 5
#elif CC4_N_HASHS_D3 == 5
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 6
#elif CC4_N_HASHS_D3 == 6
#undef CC4_N_HASHS_D3
#define CC4_N_HASHS_D3 7
#elif CC4_N_HASHS_D3 == 7
#error Sorry, number of hash functions is limited to 511.
#endif
#endif
#endif

#undef CC4_HASH
#endif

#ifdef CC4_LOAD

typedef CC4_TYPEOF_TY( CC4_1ST_ARG( CC4_LOAD ) ) CC4_CAT_3( cc4_load_, CC4_N_LOADS, _ty );

const double CC4_CAT_3( cc4_load_, CC4_N_LOADS, _val ) = CC4_OTHER_ARGS( CC4_LOAD );

#if CC4_N_LOADS_D1 == 0
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 1
#elif CC4_N_LOADS_D1 == 1
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 2
#elif CC4_N_LOADS_D1 == 2
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 3
#elif CC4_N_LOADS_D1 == 3
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 4
#elif CC4_N_LOADS_D1 == 4
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 5
#elif CC4_N_LOADS_D1 == 5
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 6
#elif CC4_N_LOADS_D1 == 6
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 7
#elif CC4_N_LOADS_D1 == 7
#undef CC4_N_LOADS_D1
#define CC4_N_LOADS_D1 0
#if CC4_N_LOADS_D2 == 0
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 1
#elif CC4_N_LOADS_D2 == 1
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 2
#elif CC4_N_LOADS_D2 == 2
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 3
#elif CC4_N_LOADS_D2 == 3
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 4
#elif CC4_N_LOADS_D2 == 4
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 5
#elif CC4_N_LOADS_D2 == 5
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 6
#elif CC4_N_LOADS_D2 == 6
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 7
#elif CC4_N_LOADS_D2 == 7
#undef CC4_N_LOADS_D2
#define CC4_N_LOADS_D2 0
#if CC4_N_LOADS_D3 == 0
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 1
#elif CC4_N_LOADS_D3 == 1
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 2
#elif CC4_N_LOADS_D3 == 2
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 3
#elif CC4_N_LOADS_D3 == 3
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 4
#elif CC4_N_LOADS_D3 == 4
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 5
#elif CC4_N_LOADS_D3 == 5
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 6
#elif CC4_N_LOADS_D3 == 6
#undef CC4_N_LOADS_D3
#define CC4_N_LOADS_D3 7
#elif CC4_N_LOADS_D3 == 7
#error Sorry, number of load factors is limited to 511.
#endif
#endif
#endif

#undef CC4_LOAD
#endif

#endif