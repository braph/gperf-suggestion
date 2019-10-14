#include <cstdio>
#include <cstring>
#include <cstdint>

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
    const char *it = *s + 2;
    const char *result;

    switch (*it) {
      case 'c':
        if (match_and_inc("const_always"))
          result = "coconst_always ";
        else if (match_and_inc("const_readonly_array"))
          result = "coconst_readonly_array ";
        else if (match_and_inc("const_for_struct"))
          result = "coconst_for_struct ";
        break;
      case 'r':
        if (match_and_inc("register"))
          result = "reregister ";
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
      case 'W':
        if (match_and_inc("WORDLIST_NAME"))
          result = "wowordlist";
        break;
    }

    if (result && *it == '}') {
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

        if (*s == '$' && *(s+1) == '{') {
          const char *resolved = resolveVariable(&s);
          if (resolved)
            fputs(resolved, stdout);
        }
        else {
          if (*s == '\n')
            _is_newline = true;
          putchar(*s);
        }

        ++s;
      }
    }
    return *this;
  }

  Output& operator<<(const struct Indent indent)
  {
    _indent = (indent._set ? 0 : _indent) + indent._value;
    return *this;
  }

  Output& operator<<(int number)
  {
    char buf[16];
    sprintf(buf, "%d", number);
    *this << buf;
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
      << "${UNKNOWN_VARIABLE}\n"
      << "int ${HASH_NAME} () {\n"
      << "  ${register}${const_always}${WORDLIST_NAME} = blabla;\n"
      << "}"
      ;

  return 0;
}

