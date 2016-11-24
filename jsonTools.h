/*

The MIT License (MIT)

Copyright (c) 2000-2016 ������ ��������� aka Keva
  keva@meta.ua
  keva@rambler.ru
  skype: big_keva
  phone: +7(495)648-4058, +7(916)015-5592

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

=============================================================================

������ �������� ��������� �����, ���������� ����� ������� ������������ �����������
� ������������� ������������ (� ���������� ���������� ������������ �����������),
������������ ������������ ����������� ����������� ��� �����������, ������� ��������������
����� �� �������������, �����������, ���������, �������, ����������, ���������������,
����������������� �/��� ������� ����� ������������ �����������, � ����� �����, �������
��������������� ������ ����������� �����������, ��� ���������� ��������� �������:

��������� ���� ����������� �� ��������� ����� � ������ ������� ������ ���� �������� ��
��� ����� ��� �������� ����� ������� ������������ �����������.

������ ����������� ����������� ��������������� ���� ���ܻ, ��� �����-���� ��������,
���� ���������� ��� ���������������, ������� �������� �������� �����������,
������������ �� ��� ����������� ���������� � ���������� ���������, �� �� �������������
���.

�� � ����� ������ ������ ��� ��������������� �� ����� ��������������� �� �����-���� �����,
�� ����� ��� �� ���� �����������, � ��� �����, ��� �������� ���������, ������� ��� ����
��������, ��������� ��-�� ������������� ������������ ����������� ��� ���� ��������
� ����������� ������������.

*/
# if !defined( __jsonTools_h__ )
# define __jsonTools_h__
# include "charstream.h"
# include "zarray.h"
# include <string.h>

namespace mtc
{
  /*
    revive - ���������� ����� ��� �����������, ��������� ��������� ��� ������� ���������;
    ��� �������� � ��� ���, �� ���������� ����� ������������ ����� �����
  */
  struct jsonRevive
  {
    unsigned    k_type;
    unsigned    v_type;
    jsonRevive* listed;
    jsonRevive* nested;

  protected:
    unsigned    cchkey;
    char        thekey[1];

  public:     // construction
    jsonRevive( unsigned    kt, unsigned    vt,
                const void* pk, unsigned    lk,
                jsonRevive* pn, jsonRevive* ps ): k_type( kt ), v_type( vt ), listed( pn ), nested( ps ), cchkey( lk )
    {
      if ( pk != nullptr )
        memcpy( thekey, pk, lk );
    }

  public:     // closed destructor
   ~jsonRevive()
    {
      if ( listed != nullptr )
        delete listed;
      if ( nested != nullptr )
        delete nested;
    }

  public:     // creation
    template <class M = def_alloc<>>
    static  jsonRevive* Create( unsigned k, unsigned v, const void* p, unsigned l, jsonRevive* n, jsonRevive* s )
      {
        jsonRevive* palloc;

        if ( (palloc = (jsonRevive*)M().alloc( sizeof(*palloc) + l - sizeof(palloc->thekey) )) != nullptr )
          new( palloc ) jsonRevive( k, v, p, l, n, s );
        return palloc;
      }
    const  jsonRevive*  Search( unsigned  k ) const
      {
        byte_t  thekey[4];                                                                                \
        return search( thekey, zarray_int_to_key( thekey, k ), z_word32 );
      }
    const  jsonRevive*  Search( const char* k ) const
      {  return search( k, w_strlen( k ), z_charstr );  }
    const  jsonRevive*  Search( const widechar* k ) const
      {  return search( k, sizeof(widechar) * w_strlen( k ), z_widestr );  }

  protected:  // search helper
    const   jsonRevive* search( const void* k, unsigned l, unsigned t ) const
      {
        return cchkey == l && memcmp( thekey, k, l ) == 0 && k_type == t ? this
          : listed != nullptr ? listed->search( k, l, t ) : nullptr;
      }

  };

# define  derive_revive( _type_ )                                                                             \
  template <class M = def_alloc<>>  jsonRevive* add_##_type_( unsigned thekey, jsonRevive* fsnext = nullptr ) \
  {                                                                                                           \
    byte_t  strkey[4];                                                                                        \
    return jsonRevive::Create<M>( z_word32, z_##_type_, strkey, zarray_int_to_key( strkey, thekey ), fsnext, nullptr );   \
  }                                                                                                                       \
  template <class M = def_alloc<>>  jsonRevive* add_##_type_( const char* thekey, jsonRevive* fsnext = nullptr )      \
  {                                                                                                                   \
    return jsonRevive::Create<M>( z_charstr, z_##_type_, thekey, strlen( thekey ), fsnext, nullptr );                 \
  }                                                                                                                   \
  template <class M = def_alloc<>>  jsonRevive* add_##_type_( const widechar* thekey, jsonRevive* fsnext = nullptr )  \
  {                                                                                                                   \
    return jsonRevive::Create<M>( z_widestr, z_##_type_, thekey, sizeof(widechar) * w_strlen( thekey ), fsnext, nullptr );  \
  }
  derive_revive( char )
  derive_revive( byte )
  derive_revive( int16 )
  derive_revive( word16 )
  derive_revive( int32 )
  derive_revive( word32 )
  derive_revive( int64 )
  derive_revive( word64 )
  derive_revive( float )
  derive_revive( double )
  derive_revive( charstr )
  derive_revive( widestr )
  derive_revive( buffer )
  derive_revive( array_char )
  derive_revive( array_byte )
  derive_revive( array_int16 )
  derive_revive( array_word16 )
  derive_revive( array_int32 )
  derive_revive( array_word32 )
  derive_revive( array_int64 )
  derive_revive( array_word64 )
  derive_revive( array_float )
  derive_revive( array_double )
  derive_revive( array_charstr )
  derive_revive( array_widestr )
  derive_revive( array_buffer )
# undef derive_revive

# define  derive_revive( _type_ )                                                                                                   \
  template <class M = def_alloc<>>  jsonRevive* add_##_type_( unsigned  thekey, jsonRevive* nested, jsonRevive* fsnext = nullptr )  \
  {                                                                                                                                 \
    byte_t  strkey[4];                                                                                                              \
    return jsonRevive::Create<M>( z_word32, z_##_type_, strkey, zarray_int_to_key( strkey, thekey ), fsnext, nested );                \
  }                                                                                                                                   \
  template <class M = def_alloc<>>  jsonRevive*  add_##_type_( const char* szname, jsonRevive* nested, jsonRevive* fsnext = nullptr ) \
  {                                                                                                                                   \
    return jsonRevive::Create<M>( z_charstr, z_##_type_, szname, strlen( szname ), fsnext, nested );                                  \
  }                                                                                                                                   \
  template <class M = def_alloc<>>  jsonRevive*  add_##_type_( const widechar* szname, void* nested, void* fsnext = nullptr ) \
  {                                                                                                                           \
    return jsonRevive::Create<M>( z_widestr, z_##_type_, szname, sizeof(widechar) * w_strlen( szname ), fsnext, nested );     \
  } 
  derive_revive( zarray )
  derive_revive( array_zarray )
  
# undef derive_revive

  /*
    charstream - ����� ��� ����������� ������ �� ������ � ������������� ������������,
    ����������� ������ getchar() � putchar(). ������� ����� ������������ ����� 2 :)
  */
  template <class S>
  struct jsonstream: public charstream<S>
  {
    jsonstream( S* s ): charstream<S>( s )  {}
    jsonstream& operator = ( S* s ) {  return (jsonstream&)charstream<S>::operator = ( s );  }

  public:     // override
    jsonstream& putchar( char c ) {  return (jsonstream&)charstream<S>::putchar( c );  }

  public:     // gets
    /*
      get first non-space character
    */
    char  nospace()
      {
        for ( ; ; )
        {
          char  getchr;
          char  chnext;

          if ( (getchr = getchar()) == '\0' )
            return getchr;

          if ( (unsigned char)getchr <= 0x20 )
            continue;

          if ( getchr != '/' )
            return getchr;

          switch ( chnext = getchar() )
          {
            case '*':
              {
                for ( getchr = '/'; ; getchr = chnext )
                {
                  if ( (chnext = getchar()) == '\0')
                    return chnext;
                  if ( chnext == '/' && getchr == '*' )
                    break;
                }
                break;
              }
            case '/':
              {
                while ( (getchr = getchar()) != '\0' && getchr != '\n' )
                  (void)NULL;
                if ( getchr == '\0' ) return getchr;
                  else break;
              }

            case '\0':  return getchr;
            default:    putchar( chnext );
                        return getchr;
          }
        }
      }
    bool  getfour( char* p )
      {
        return (*p++ = nospace()) != '\0' && (*p++ = getchar()) != '\0'
            && (*p++ = getchar()) != '\0' && (*p++ = getchar()) != '\0';
      }
    template <class C, class M>
    int   getstring( _auto_<C, M>&  refstr, const unsigned maxval = (1 << (sizeof(C) * CHAR_BIT)) - 1 )
      {
        char  chnext;
        char  chprev;
        int   cchstr = 0;
        int   climit = 0;

        if ( (chnext = nospace()) != '\"' )
          return EINVAL;

        for ( chprev = '\0'; (chnext = getchar()) != '\0'; chprev = chnext )
        {
        // check for end of identifier
          if ( chnext == '\"' && chprev != '\\' )
            return 0;

        // check for '\\'
          if ( chnext == '\\' && chprev != '\\' )
            continue;

        // check regular char
          if ( chprev != '\\' )
            if ( append( refstr, (C)chnext, cchstr, climit ) != 0 ) return ENOMEM;
              else continue;

        // check long code: 4 symbols
          if ( chnext == 'u' )
          {
            char      hexchr[5] = "    ";
            widechar  uvalue;
            char*     endptr;

            if ( getfour( hexchr ) )  uvalue = (widechar)strtoul( hexchr, &endptr, 0x10 );
              else  return EINVAL;
            if ( endptr - hexchr != 4 || uvalue > maxval )
              return EINVAL;
            if ( append( refstr, (C)uvalue, cchstr, climit ) != 0 )
              return ENOMEM;
          }
            else
          switch ( chnext )
          {
            case 'b':   if ( append( refstr, (C)'\b', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case 't':   if ( append( refstr, (C)'\t', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case 'n':   if ( append( refstr, (C)'\n', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case 'f':   if ( append( refstr, (C)'\f', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case 'r':   if ( append( refstr, (C)'\r', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case '\"':  if ( append( refstr, (C)'\"', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case '/':   if ( append( refstr, (C)'/' , cchstr, climit ) == 0 )  break;  else return ENOMEM;
            case '\\':  if ( append( refstr, (C)'\\', cchstr, climit ) == 0 )  break;  else return ENOMEM;
            default:                                                                     return EINVAL;
          }
          chprev = '\0';
        }
        return EINVAL;
      }

  protected:  // helpers
    template <class C, class M>
    int   append( _auto_<C, M>& refstr, C c, int& cchstr, int& climit )
      {
        if ( cchstr >= climit - 1 )
        {
          _auto_<C, M>  newstr;
          int           newlim;

          if ( (newstr = (C*)M().alloc( (newlim = climit + 0x100) * sizeof(C) )) == nullptr )
            return ENOMEM;
          if ( refstr != nullptr )
            memcpy( (C*)newstr, (C*)refstr, sizeof(C) * (cchstr + 1) );
          climit = newlim;
          refstr = newstr;
        }

        refstr[cchstr++] = c;
        refstr[cchstr+0] = (C)0;
        return 0;
      }
  };

  /*
    helper functions
  */
  inline  bool  wstrtodbl( double& o, const widechar* s )
  {
    widechar* endptr;
      o = w_strtod( s, &endptr );
    return *endptr == 0;
  }

  inline  bool  wstrtoint( unsigned& o, const widechar* s )
  {
    widechar* endptr;
      o = w_strtol( s, &endptr, 10 );
    return *endptr == 0 && endptr > s;
  }

  template <class M>
  inline  bool  wstrtostr( _auto_<char, M>& o, const widechar* s )
  {
    _auto_<char, M> a;
    char*           p;

    if ( (p = a = (char*)M().alloc( sizeof(char) * (1 + w_strlen( s )))) == nullptr )
      return false;

    while ( *s != 0 && *s < 0x100 )
      *p++ = (char)*s++;

    if ( *s == 0 )  {  *p = '\0';  o = a;  return true;  }
      else return false;
  }

// JSON decoration modes

  struct json_compact
  {
    template <class O>  O*  Shift( O* o ) const {  return o;  }
    template <class O>  O*  Space( O* o ) const {  return o;  }
    template <class O>  O*  Break( O* o ) const {  return o;  }
  };

  class json_decorated
  {
    int   nlevel;

  public:     // construction
    json_decorated(): nlevel( 0 ) {  }
    json_decorated( const json_decorated& js ): nlevel( js.nlevel + 1 ) {  }

  public:     // shifts
    template <class O>  O*  Shift( O* o ) const
      {
        for ( auto n = 0; o != nullptr && n < nlevel; ++n )
          o = ::Serialize( o, "  ", 2 );
        return o;
      }
    template <class O>  O*  Space( O* o ) const
      {
        return ::Serialize( o, ' ' );
      }
    template <class O>  O*  Break( O* o ) const
      {
        return ::Serialize( o, '\n' );
      }
  };

// JSON serialization

  # define  derive_printjson_dec( _type_, _tmpl_ )                                      \
  template <class O, class D = json_compact>                                            \
  inline  O*  PrintJson( O* o, _type_ t, const D& decorate = D() )                      \
    {                                                                                   \
      char  decval[0x10];                                                               \
      return ::Serialize( decorate.Shift( o ), decval, sprintf( decval, _tmpl_, t ) );  \
    }
    derive_printjson_dec( char,   "%d" )
    derive_printjson_dec( byte_t, "%u" )
    derive_printjson_dec( int16_t,  "%d" )
    derive_printjson_dec( word16_t, "%u" )
    derive_printjson_dec( int32_t,  "%d" )
    derive_printjson_dec( word32_t, "%u" )
  # if defined( _MSC_VER )
    derive_printjson_dec( int64_t,  "%I64d" )
    derive_printjson_dec( word64_t, "%I64u" )
  # else
    derive_printjson_dec( int64_t,  "%lld" )
    derive_printjson_dec( word64_t, "%llu" )
  # endif  // _MSC_VER
  # undef derive_printjson_dec

  # define  derive_printjson_flo( _type_ )                                            \
    template <class O, class D = json_compact>                                        \
    inline  O*  PrintJson( O* o, _type_ t, const D& decorate = D() )                  \
    {                                                                                 \
      char  floval[0x10];                                                             \
      return ::Serialize( decorate.Shift( o ), floval, sprintf( floval, "%f", t ) );  \
    }
    derive_printjson_flo( float )
    derive_printjson_flo( double )
  # undef derive_printjson_flo

  template <class O, class C>
  inline  O*  PrintText( O* o, const C* s, size_t l )
  {
    static char         repsrc[] = "\b\t\n\f\r\"/\\";
    static const char*  repval[] = { "\\b", "\\t", "\\n", "\\f", "\\r", "\\\"", "\\/", "\\\\" };
    char*               reppos;
    char                chnext[0x10];

    if ( s == nullptr )
      return ::Serialize( o, "null", 4 );

    for ( o = ::Serialize( o, '\"' ); l-- > 0; ++s )
    {
      if ( (*s & ~0xff) == 0 && (reppos = strchr( repsrc, *s )) != nullptr )
        o = ::Serialize( o, repval[reppos - repsrc], strlen( repval[reppos - repsrc] ) );   else
      if ( (unsigned)*s >= 0x80 || (unsigned)*s < 0x20 )
        o = ::Serialize( o, chnext, sprintf( chnext, "\\u%04x", (unsigned)*s ) );           else
      o = ::Serialize( o, (char)*s );
    }

    return ::Serialize( o, '\"' );
  }

  template <class O, class D = json_compact>
  inline  O*  PrintJson( O* o, const char* s, const D& decorate = D() )
    {  return PrintText( o, (const unsigned char*)s, w_strlen( s ) );  }
  template <class O, class D = json_compact>
  inline  O*  PrintJson( O* o, const widechar* s, const D& decorate = D() )
    {  return PrintText( o, s, w_strlen( s ) );  }
  template <class O, class D = json_compact>
  inline  O*  PrintJson( O* o, const _auto_<char>& s, const D& decorate = D() )
    {  return PrintText( o, (const unsigned char*)(const char*)s, w_strlen( s ) );  }
  template <class O, class D = json_compact>
  inline  O*  PrintJson( O* o, const _auto_<widechar>& s, const D& decorate = D() )
    {  return PrintText( o, (const widechar*)s, w_strlen( s ) );  }

  template <class O, class M, class D = json_compact>  O*  PrintJson( O*, const xvalue<M>&, const D& decorate = D() );
  template <class O, class M, class D = json_compact>  O*  PrintJson( O*, const zarray<M>&, const D& decorate = D() );

// arrays
  template <class O, class T, class M, class D = json_compact>
  inline  O*  PrintJson( O* o, const array<T, M>& a, const D& decorate = D() )
  {
    auto  ptop = a.begin();
    auto  pend = a.end();

    for ( o = decorate.Break( ::Serialize( decorate.Shift( o ), '[' ) ); o != nullptr && ptop < pend; ++ptop )
    {
      o = decorate.Break( ::Serialize( PrintJson( o, *ptop, D( decorate ) ), ",", ptop < pend - 1 ? 1 : 0 ) );
    }
    return ::Serialize( decorate.Shift( o ), ']' );
  }

  template <class O, class M, class D>
  inline  O*  PrintJson( O* o, const xvalue<M>& v, const D& decorate )
  {
    switch ( v.gettype() )
    {
      case z_char:    return PrintJson( decorate.Space( o ), *v.get_char() );
      case z_byte:    return PrintJson( decorate.Space( o ), *v.get_byte() );
      case z_int16:   return PrintJson( decorate.Space( o ), *v.get_int16() );
      case z_word16:  return PrintJson( decorate.Space( o ), *v.get_word16() );
      case z_int32:   return PrintJson( decorate.Space( o ), *v.get_int32() );
      case z_word32:  return PrintJson( decorate.Space( o ), *v.get_word32() );
      case z_int64:   return PrintJson( decorate.Space( o ), *v.get_int64() );
      case z_word64:  return PrintJson( decorate.Space( o ), *v.get_word64() );
      case z_float:   return PrintJson( decorate.Space( o ), *v.get_float() );
      case z_double:  return PrintJson( decorate.Space( o ), *v.get_double() );

      case z_charstr: return PrintJson( decorate.Space( o ), v.get_charstr() );
      case z_widestr: return PrintJson( decorate.Space( o ), v.get_widestr() );
      case z_zarray:  return PrintJson( decorate.Break( o ), *v.get_zarray(), D( decorate ) );

      case z_array_char:    return PrintJson( decorate.Break( o ), *v.get_array_char(),   D( decorate ) );
      case z_array_byte:    return PrintJson( decorate.Break( o ), *v.get_array_byte(),   D( decorate ) );
      case z_array_int16:   return PrintJson( decorate.Break( o ), *v.get_array_int16(),  D( decorate ) );
      case z_array_word16:  return PrintJson( decorate.Break( o ), *v.get_array_word16(), D( decorate ) );
      case z_array_int32:   return PrintJson( decorate.Break( o ), *v.get_array_int32(),  D( decorate ) );
      case z_array_word32:  return PrintJson( decorate.Break( o ), *v.get_array_word32(), D( decorate ) );
      case z_array_int64:   return PrintJson( decorate.Break( o ), *v.get_array_int64(),  D( decorate ) );
      case z_array_word64:  return PrintJson( decorate.Break( o ), *v.get_array_word64(), D( decorate ) );
      case z_array_float:   return PrintJson( decorate.Break( o ), *v.get_array_float(),  D( decorate ) );
      case z_array_double:  return PrintJson( decorate.Break( o ), *v.get_array_double(), D( decorate ) );

      case z_array_charstr: return PrintJson( o, *v.get_array_charstr() );
      case z_array_widestr: return PrintJson( o, *v.get_array_widestr() );
      case z_array_zarray:  return PrintJson( decorate.Break( o ), *v.get_array_zarray(), D( decorate ) );
      case z_array_xvalue:  return PrintJson( o, *v.get_array_xvalue() );

      default:  assert( false );  abort();  return o;
    }
  }

  template <class O, class M, class D>
  inline  O*  PrintJson( O* o, const zarray<M>& v, const D& decorate )
  {
    bool  bcomma = false;

    o = decorate.Break( ::Serialize( decorate.Shift( o ), '{' ) );

    v.for_each( [&]( const typename zarray<M>::zkey& k, const xvalue<M>& v )
      {
        D   subdecor( decorate );

      // possible comma
        if ( bcomma )
          o = subdecor.Break( ::Serialize( o, ',' ) );

      // key
        if ( (const char*)k != nullptr )      o = PrintJson( subdecor.Shift( o ), (const char*)k );  else
        if ( (const widechar*)k != nullptr )  o = PrintJson( subdecor.Shift( o ), (const widechar*)k );  else
                                            o = ::Serialize( PrintJson( ::Serialize( subdecor.Shift( o ), '"' ), (unsigned)k ), '"' );

      // value
        o = PrintJson( ::Serialize( o, ':' ), v, subdecor );
        bcomma = true;
        return 0;
      } );

    return ::Serialize( decorate.Shift( bcomma ? decorate.Break( o ) : o ), '}' );
  }

  /*
    Parsing:
      - ordinal values;
  */

  // unsigned integers
  # define  derive_parse( _type_ )                                                \
    template <class S>  S*  ParseJson( jsonstream<S>& s, _type_& v )              \
    {                                                                             \
      char  chnext;                                                               \
      if ( (chnext = s.nospace()) < '0' || chnext > '9' )                         \
        return nullptr;                                                           \
      for ( v = (byte_t)chnext - '0'; (chnext = s.getchar()) >= '0' && chnext <= '9'; )  \
        v = v * 10 + (byte_t)chnext - '0';                                        \
      return s.putchar( chnext );                                                 \
    }
    derive_parse( byte_t )
    derive_parse( word16_t )
    derive_parse( word32_t )
    derive_parse( word64_t )
  # undef derive_parse

  // signed integers
  # define  derive_parse( _type_ )                                                \
  template <class S>  S*  ParseJson( jsonstream<S>& s, _type_& v )                \
  {                                                                               \
    char  chnext;                                                                 \
    bool  is_neg;                                                                 \
    if ( is_neg = ((chnext = s.nospace()) == '-') )                               \
      chnext = s.getchar();                                                       \
    if ( chnext < '0' || chnext > '9' )                                           \
      return nullptr;                                                             \
    for ( v = (byte_t)chnext - '0'; (chnext = s.getchar()) >= '0' && chnext <= '9'; )  \
      v = v * 10 + (byte_t)chnext - '0';                                          \
    if ( is_neg ) v = -v;                                                         \
      return s.putchar( chnext );                                                 \
  }
    derive_parse( char )
    derive_parse( int16_t )
    derive_parse( int32_t )
    derive_parse( int64_t )
  # undef derive_parse

  // floating
  # define  derive_parse( _type_ )                                                \
    template <class S>  S*  ParseJson( jsonstream<S>& s, _type_&  v )             \
    {                                                                             \
      char  flobuf[0x40];                                                         \
      char* floptr;                                                               \
      char* endptr;                                                               \
      for ( *(floptr = flobuf) = s.nospace(); floptr < flobuf + 0x40 && *floptr != '\0'        \
        && strchr( "0123456789.-eE", *floptr ) != nullptr; *++floptr = s.getchar() )  (void)NULL; \
      v = (_type_)strtod( flobuf, &endptr );                                                    \
      return endptr == floptr && floptr < array_end( flobuf ) ? s.putchar( *floptr ) : nullptr; \
    }
    derive_parse( float )
    derive_parse( double )
  # undef derive_parse

  // arrays
  # define  derive_fetch_array( _type_ )                                              \
    template <class S, class M> S*  ParseJson( jsonstream<S>& s, array<_type_, M>& a ) \
    {                                                                                 \
      char  chnext;                                                                   \
      if ( (chnext = s.nospace()) != '[' )                                            \
        return nullptr;                                                               \
      while ( (chnext = s.nospace()) != '\0' && chnext != ']' )                       \
      {                                                                               \
        _type_  avalue;                                                               \
                                                                                      \
        if ( ParseJson( s.putchar( chnext ), avalue ) == nullptr )                    \
          return nullptr;                                                             \
        if ( a.Append( avalue ) != 0 )                                                \
          return nullptr;                                                             \
        if ( (chnext = s.nospace()) == ',' )                                          \
          continue;                                                                   \
        if ( chnext == ']' )  s.putchar( chnext );                                    \
          else return nullptr;                                                        \
      }                                                                               \
      return chnext == ']' ? s : nullptr;                                             \
    }
    derive_fetch_array( char )
    derive_fetch_array( byte_t )
    derive_fetch_array( int16_t )
    derive_fetch_array( int32_t )
    derive_fetch_array( int64_t )
    derive_fetch_array( word16_t )
    derive_fetch_array( word32_t )
    derive_fetch_array( word64_t )
    derive_fetch_array( float )
    derive_fetch_array( double )
    derive_fetch_array( zarray<M> )
  # undef derive_fetch_array

  template <class S, class C, class M>
  S*  ParseJson( jsonstream<S>& s, array<_auto_<C, M>, M>& a )
  {
    char  chnext;

    if ( (chnext = s.nospace()) != '[' )
      return nullptr;
    while ( (chnext = s.nospace()) != '\0' && chnext != ']' )
    {
      _auto_<C, M>  sznext;

      if ( chnext != '\"' || s.putchar( chnext ).getstring( sznext ) != 0 || a.Append( sznext ) != 0 )
        return nullptr;
      if ( (chnext = s.nospace()) == ',' )
        continue;
      if ( chnext == ']' )  s.putchar( chnext );
        else return nullptr;
    }
    return chnext == ']' ? s : nullptr;
  }

  template <class S, class T, class M>
  S*  ParseJson( jsonstream<S>& s, array<T, M>& a, const jsonRevive* p = nullptr )
  {
    char  chnext;

    if ( (chnext = s.nospace()) != '[' )
      return nullptr;
    while ( (chnext = s.nospace()) != '\0' && chnext != ']' )
    {
      T t;

      if ( (S*)(s = ParseJson( s.putchar( chnext ), t, p )) == nullptr )
        return nullptr;
      if ( a.Append( t ) != 0 )
        return nullptr;

      if ( (chnext = s.nospace()) == ',' )
        continue;
      if ( chnext == ']' )  s.putchar( chnext );
        else return nullptr;
    }
    return chnext == ']' ? s : nullptr;
  }

  template <class S, class M>
  S*  ParseJson( jsonstream<S>& s, xvalue<M>& x, const jsonRevive* p )
  {
    const byte_t  vatype = p != nullptr ? p->v_type : 0xff;
    char          chnext;

    switch ( vatype )
    {
      case z_char:     return ParseJson( s, *x.set_char() );
      case z_int16:    return ParseJson( s, *x.set_int16() );
      case z_int32:    return ParseJson( s, *x.set_int32() );
      case z_int64:    return ParseJson( s, *x.set_int64() );
      case z_byte:     return ParseJson( s, *x.set_byte() );
      case z_word16:   return ParseJson( s, *x.set_word16() );
      case z_word32:   return ParseJson( s, *x.set_word32() );
      case z_word64:   return ParseJson( s, *x.set_word64() );
      case z_float:    return ParseJson( s, *x.set_float() );
      case z_double:   return ParseJson( s, *x.set_double() );
      case z_charstr:  return s.getstring( *(_auto_<char, M>*)&x.set_charstr() ) == 0 ? s : nullptr;
      case z_widestr:  return s.getstring( *(_auto_<widechar, M>*)&x.set_widestr() ) == 0 ? s : nullptr;
//      case z_buffer:   return ParseJsonStore( o, *(_freebuffer_*)&chdata );
      case z_zarray:   return ParseJson( s, *x.set_zarray() );

      case z_array_char:   return ParseJson( s, *x.set_array_char() );
      case z_array_byte:   return ParseJson( s, *x.set_array_byte() );
      case z_array_int16:  return ParseJson( s, *x.set_array_int16() );
      case z_array_int32:  return ParseJson( s, *x.set_array_int32() );
      case z_array_int64:  return ParseJson( s, *x.set_array_int64() );
      case z_array_word16: return ParseJson( s, *x.set_array_word16() );
      case z_array_word32: return ParseJson( s, *x.set_array_word32() );
      case z_array_word64: return ParseJson( s, *x.set_array_word64() );
      case z_array_float:  return ParseJson( s, *x.set_array_float() );
      case z_array_double: return ParseJson( s, *x.set_array_double() );
      case z_array_zarray: return ParseJson( s, *x.set_array_zarray(), p != nullptr ? p->nested : nullptr );
      case z_array_charstr: return ParseJson( s, *x.set_array_charstr() );
      case z_array_widestr: return ParseJson( s, *x.set_array_widestr() );
/*      case z_array_buffer: return ::JsonStore( o, *(array_buffer*)&chdata );
      case z_array_xvalue: return ::JsonStore( o, *(array_xvalue*)&chdata );
  */
      default:  break;
    }

  // untyped xvalue load: object zarray {...}, untyped array [...], string "...", integer -?[0-9]+ or float -?[0-9]+\.?[0-9]*([Ee]-?[0-9]+)?
    switch ( chnext = s.nospace() )
    {
      case '{':   return ParseJson( s.putchar( chnext ), *x.set_zarray(), p );
      case '[':   return ParseJson( s.putchar( chnext ), *x.set_array_xvalue(), p );
      case '\"':
        {
          _auto_<widechar, M> wcsstr;
          _auto_<char, M>     chrstr;
          double              dblval;
          unsigned            intval;

        // get next string
          if ( s.putchar( chnext ).getstring( wcsstr ) != 0 )
            return nullptr;

        // check if charstr or widestr
          if ( wstrtodbl( dblval, wcsstr ) )  x.set_double( dblval );   else
          if ( wstrtoint( intval, wcsstr ) )  x.set_word32( intval );   else
          if ( wstrtostr( chrstr, wcsstr ) )  x.set_charstr( chrstr );  else                              
                                              x.set_widestr( wcsstr );
          return s;
        }
      default:  break;
    }
    if ( (chnext >= '0' && chnext <= '9') || chnext == '-' )
    {
      array<char, M>  cvalue;
      bool            bpoint;
      bool            bexpon;
      char            chprev;

      if ( cvalue.Append( chnext ) != 0 )
        return nullptr;
      for ( chprev = '\0', bexpon = bpoint = false; ; )
      {
        if ( (chnext = s.getchar()) >= '0' && chnext <= '9' )
        {
          if ( cvalue.Append( chprev = chnext ) != 0 )
            return nullptr;
        }
          else
        if ( chnext == '.' )
        {
          if ( bpoint || bexpon || cvalue.Append( chprev = chnext ) != 0 ) return nullptr;
            else  bpoint = true;
        }
          else
        if ( chnext == 'e' || chnext == 'E' )
        {
          if ( bexpon || cvalue.Append( chprev = chnext ) != 0 ) return nullptr;
            else  bexpon = true;
        }
          else
        if ( chnext == '-' )
        {
          if ( ( chprev != 'e' && chprev != 'E' ) || cvalue.Append( chprev = chnext ) != 0 )
            return nullptr;
        }
          else
        {
          s.putchar( chnext );
          break;
        }
      }
      if ( cvalue.Append( '\0' ) != 0 )
        return nullptr;
      if ( bpoint || bexpon )
      {
        char*   endptr;
        double  dvalue = strtod( cvalue, &endptr );

        if ( *endptr != '\0' )  return nullptr;
          else  x.set_double( dvalue );
      }
        else
      {
        char*   endptr;
        int32_t nvalue = strtol( cvalue, &endptr, 10 );

        if ( *endptr != '\0' )  return nullptr;
          else  x.set_int32( nvalue );
      }
      return s;
    }
    return nullptr;
  }

  template <class S, class M>
  S*  ParseJson( jsonstream<S>& s, zarray<M>& z, const jsonRevive* p = nullptr )
  {
    char  chnext;

  // reset
    z.DelAllData();

  // open object
    if ( (chnext = s.nospace()) != '{' )
      return nullptr;

  // char by char until end or '}'
    while ( (chnext = s.nospace()) != '\0' && chnext != '}' )
    {
      unsigned            intkey = 0;
      _auto_<char, M>     strkey;
      _auto_<widechar, M> wcskey;
      xvalue<M>*          newval = nullptr;
      const jsonRevive*   ptypes = nullptr;

    // get variable name as widestring
      if ( chnext != '\"' || s.putchar( chnext ).getstring( wcskey ) != 0 )
        return nullptr;

    // check variable name type (if defined); create key type and value type for the key
      if ( p != nullptr )
      {
        if (                                (ptypes = p->Search( wcskey )) != nullptr )  newval = z.put_xvalue( wcskey );
          else
        if ( wstrtostr( strkey, wcskey ) && (ptypes = p->Search( strkey )) != nullptr )  newval = z.put_xvalue( strkey );
          else
        if ( wstrtoint( intkey, wcskey ) && (ptypes = p->Search( intkey )) != nullptr )  newval = z.put_xvalue( intkey );
      }

    // default key create
      if ( newval == nullptr )
      {
        if (                      wstrtoint( intkey, wcskey ) )  newval = z.put_xvalue( intkey );  else
        if ( strkey != nullptr || wstrtostr( strkey, wcskey ) )  newval = z.put_xvalue( strkey );  else
                                                                 newval = z.put_xvalue( wcskey );
      }

    // check for colon
      if ( (chnext = s.nospace()) != ':' )
        return nullptr;

    // get the value
      if ( (S*)(s = ParseJson( s, *newval, ptypes )) == nullptr )
        return nullptr;

    // check for comma
      if ( (chnext = s.nospace()) == ',' )
        continue;
      if ( chnext == '}' )  s.putchar( chnext );
        else return nullptr;
    }

  // check valid script
    return chnext == '}' ? s : nullptr;
  }

  template <class S, class M>
  S*  ParseJson( S* s, xvalue<M>& x, const jsonRevive* p = nullptr )
  {
    jsonstream<S> source( s );

    return ParseJson( source, x, p );
  }

  template <class S, class M>
  S*  ParseJson( S* s, zarray<M>& z, const jsonRevive* p = nullptr )
  {
    jsonstream<S> source( s );

    return ParseJson( source, z, p );
  }

}

# endif  // __jsonTools_h__
