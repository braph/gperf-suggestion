#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib> //REMOVE ME

/* Enable/disable output using If, Else, EndIf */
#define If(EXPR) ((EXPR) ? IfTrue : IfFalse)
#define Else() IfElse
#define EndIf() IfEnd
enum IfState {
  IfTrue,
  IfFalse,
  IfElse,
  IfEnd
};

/* Set/Increment/Decrement indentation. Maximum value is 127 */
#define SetIndent(N) (Indent(true, N))
#define DecIndent(N) (Indent(false, -(N)))
#define IncIndent(N) (Indent(false, N))
struct Indent {
  bool _set;
  int8_t _value;
  Indent(bool set, int8_t value) :
    _set(set), _value(value) {}
};

/* Print N spaces, more efficient than ``printf("%.s", N, "")`` */
#define printIndent(N) (fputs(spaces + sizeof(spaces) - (N) - 1, stdout))
const char spaces[] = "                                "; // 32 spaces

#if 0
/* Output gperf's ASCII-case insensitive strncmp replacement.  */
  printf (option[KRC] | option[C] ?
               "(s1, s2, n)\n"
          "     $register $const char *s1;\n"
          "     $register $const char *s2;\n"
          "     $register size_t n;\n" :
          option[ANSIC] | option[CPLUSPLUS] ?
               "($register const char *s1, $register const char *s2, $register size_t n)\n" :
          "")

const char* function_parameters(const char *parameters, ...) {
  static const char *foo = 
  if (option[KRC] | option[C]) {

  }
}

// 33 * register_scs
// 10 * const_always
// 9  * const_readonly_array
// 2  * const_for_struct
// 4  * smallest_integral_type
// 100 * indent + ""
// 3 option.get_class_name
// 10 option.get_constants_prefix
// 2 option.get_function_name
// 3 option.get_hash_name
// 2 option.get_initializer_suffix
// 1 option.get_input_file_name
// 5 option.get_lengthtable_name
// 7 option.get_slot_name
// 14 option.get_stringpool_name
// 10 option.get_wordlist_name
// 52

#endif

#define ANSIC 1

class Output {
public:
  Output() :
    _no_output(0),
    _n(-1),
    _indent(0),
    _is_newline(1)
  {}

#define match_and_inc(WORD) (!strncmp(it, WORD, sizeof(WORD)-1) && (it += sizeof(WORD)-1))
  /* Resolve ``${VARIABLE}`` */
  const char* resolveVariable(const char **s) {
    const char *it = *s + 1;
    const char *result;
    static char buffer[256];
    bool has_brace = *it == '{';
    const char *it2;
    if (has_brace)
      ++it;

    switch (*it) {
      case 'c':
        if (match_and_inc("const_always"))
          result = "coconst_always ";
        else if (match_and_inc("const_readonly_array"))
          result = "coconst_readonly_array ";
        else if (match_and_inc("const_for_struct"))
          result = "coconst_for_struct ";
        else if (match_and_inc("const:")) {
          const char *arg1, *arg2;
          if ((arg1 = resolveVariable(&it))) {
            ++it;
            if (*it++ == ',') { // && *++it == '$' && *++it == '{')
              if ((arg2 = resolveVariable(&it))) {
                ++it;
                sprintf(buffer, "LOL:%s<>%s", arg1, arg2);
                result = buffer;
              }
            }
          }
        }
        break;
      case 's':
        if (match_and_inc("smallest_integral_type:")) {
          if (match_and_inc("max_hash_value+1"))
            result = smallest_integral_type(max_hash_value+1);
          else if (match_and_inc("max_key_len"))
            result = smallest_integral_type(max_key_len);
        }
        break;

      case 'p':
#if 0
        if (match_and_inc("parameters:")) {
          if (ANSIC)
            ;
          strchr(it, ')');
          it2 = strpbrk(it, ",}");
          ${parameters:(foo, int)\n$register $const char *s1, 
  printf (option[KRC] | option[C] ?
               "(s1, s2, n)\n"
          "     $register $const char *s1;\n"
          "     $register $const char *s2;\n"
          "     $register size_t n;\n" :
          option[ANSIC] | option[CPLUSPLUS] ?
               "($register const char *s1, $register const char *s2, $register size_t n)\n" :
          "")
        }
#endif
      case 'i':
        if (match_and_inc("if:")) {
        }
        /*
        else if (match_and_inc("index")) {
          sprintf(buffer, "%d", _index_bind);
          result = buffer;
        }
        */
        break;
        /*
      case 'k':
        if (match_and_inc("keyword")) {
          print_string(_keyword_bound->_allchars, _keyword_bound->_allchars_length);
        }
        break;
        */
      case 'r':
        if (match_and_inc("register "))
          result = "reregister ";
        break;
      case 'C':
        if (match_and_inc("CLASS_NAME"))
          result = "claclass_name";
        break;
      case 'F':
        if (match_and_inc("FUNCTION_NAME"))
          result = "fufunction";
        break;
      case 'H':
        if (match_and_inc("HASH_NAME"))
          result = "hahash";
        break;
      case 'L':
        if (match_and_inc("LENGTHTABLE_NAME"))
          result = "lelengthtable";
        break;
      case 'M':
        if (match_and_inc("MAX_WORD_LENGTH"))
          result = "";
        else if (match_and_inc("MIN_WORD_LENGTH"))
          result = "";
        else if (match_and_inc("MAX_HASH_VALUE"))
          result = "";
        else if (match_and_inc("MIN_HASH_VALUE"))
          result = "";
      case 'P':
        if (match_and_inc("PREFIX"))
          result = "preprefix";
        break;
      case 'S':
        if (match_and_inc("STRINGPOOL_NAME"))
          result = "strstringpool";
        else if (match_and_inc("SLOT_NAME"))
          result = "sloslotname";
        break;
      case 'V':
        if (match_and_inc("VERSION"))
          result = "verversion";
        break;
      case 'W':
        if (match_and_inc("WORDLIST_NAME"))
          result = "wowordlist";
        break;
    }

    if (result) {
      //printf(">>>%s\n", it);

      if (has_brace) {
        if (*it != '}') {
          return NULL;
        }
        ++it;
      }
      
      *s = it;
      return result;
    }

    return NULL;
  }

  Output& operator<<(const char *s)
  {
    if (!_no_output) {
      while (*s) {
        if (_is_newline && *s != '\n') {
          printIndent(_indent);
          _is_newline = false;
        }

        if (*s == '$') {
          const char *resolved = resolveVariable(&s);
          if (resolved) {
            fputs(resolved, stdout);
            continue;
          }
        }
        else {
          if (*s == '\n')
            _is_newline = true;
        }

        putchar(*s);
        ++s;
      }
    }
    return *this;
  }

  Output& operator<<(const struct Indent indent)
  {
    if (!_no_output)
      _indent = (indent._set ? 0 : _indent) + indent._value;
    return *this;
  }

  Output& operator<<(int number)
  {
    if (!_no_output) {
      char buf[16];
      sprintf(buf, "%d", number);
      *this << buf;
    }
    return *this;
  }

  /* Maximum 32 nested IFs possible */
  Output& operator<<(enum IfState ifState)
  {
    switch (ifState) {
      case IfTrue:
        _if_stack[++_n] = IfTrue;
        break;
      case IfFalse:
        _if_stack[++_n] = IfFalse;
        _no_output |= (1U << _n);
        break;
      case IfElse:
        if (_if_stack[_n] == IfTrue) {
          _no_output |= (1U << _n);
          _if_stack[_n] = IfElse;
        }
        else if (_if_stack[_n] == IfFalse) {
          _no_output &= ~(1U << _n);
          _if_stack[_n] = IfElse;
        }
        else {
          printf("<Else> without <If>\n");
        }
        break;
      case IfEnd:
        _no_output &= ~(1U << _n);
        --_n;
        break;
    }
    return *this;
  }

private:
  uint32_t _no_output;
  enum IfState _if_stack[32];
  int _n;
  unsigned int _indent;
  bool _is_newline;
};

int main(int argc, char **argv)
{
  Output out;

  out << "Test 1: "
      << If (true)
      << If (false)
      << "failed\n"
      << Else ()
      << "passed\n"
      << EndIf ()
      << "Test 2: "
      << Else ()
      << "failed\n"
      << EndIf ()
      << "passed\n"
      << SetIndent(2)
      << "Indent: 2\n"
      << IncIndent(2)
      << "Indent: 4\n"
      << DecIndent(4)
      << "Indent: 0\n"
      << SetIndent(4)
      << "indent this"
      << " -- but don't indent that\n"
      << "indent this\n"
         " -- also indent that\n"
      << "please, don't indent these empty lines:\n\n\n"
      << "\n"
      << "\n"
      << "A number: " << 42 << "\n"
      << "$register ${UNKNOWN_VARIABLE}\n"
      << "int ${HASH_NAME} () {\n"
      << "  ${const:${const_always},${WORDLIST_NAME}}\n"
      << "  ${register}${const_always}${WORDLIST_NAME} = blabla;\n"
      << "}\n"
      << strdup("MEPMEPHAHAHA\n")
      ;

  return 0;
}

