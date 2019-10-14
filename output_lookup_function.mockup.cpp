/* This is how the code could look like */

void
Output::output_lookup_function_body (const Output_Compare& comparison) const
{
  *this
    << "  if (len <= ${PREFIX}MAX_WORD_LENGTH && len >= ${PREFIX}MIN_WORD_LENGTH)\n"
       "    {\n"
       "      ${register} unsigned int key = ${HASH_NAME} (str, len);\n\n";

  if (option[SWITCH])
    {
      int switch_size = num_hash_values ();
      int num_switches = option.get_total_switches ();
      if (num_switches > switch_size)
        num_switches = switch_size;

      *this
        << SetIndent (4)
        << "  if (key <= ${PREFIX}MAX_HASH_VALUE"
        << (_min_hash_value > 0 ? " && key >= ${PREFIX}MIN_HASH_VALUE" : "")
        << ")\n"
           "    {\n";
        << If (option[DUP] && _total_duplicates > 0)
        << If (option[LENTABLE])
        << "      ${register} ${const_always} " << smallest_integral_type(_max_key_len) << " *lengthptr;\n"
        << EndIf () // LENTABLE
        << "      ${register} " << output_const_type (const_readonly_array, _wordlist_eltype) << " *wordptr;\n"
        << "      ${register} " << output_const_type (const_readonly_array, _wordlist_eltype) << " *wordendptr;\n"
        << EndIf () // DUP && _total_duplicates > 0
        << If(option[TYPE])
        << "      ${register}" << output_const_type (const_readonly_array, _struct_tag) << " *resword;\n\n"
        << Else () // If (!TYPE)
        << "      ${register} ${struct_tag} resword;\n\n"
        << EndIf (); // TYPE
      output_switches (_head, num_switches, switch_size, _min_hash_value, _max_hash_value, 10);
      *this
        << "      return 0;\n";
        << If (option[DUP] && _total_duplicates > 0)
        << SetIndent (8)
        << "multicompare:\n"
           "  while (wordptr < wordendptr)\n"
           "    {\n"
        << If (option[LENTABLE])
        << "      if (len == *lengthptr)\n"
           "        {\n",
        << IncIndent (4)
        << EndIf () // LENTABLE
        << "      ${register} ${const_always} char *s = "
        << (option[TYPE] ? "wordptr->${SLOT_NAME}" : "*wordptr")
        << (option[SHAREDLIB] ? " + ${STRINGPOOL_NAME}" : "")
        << ";\n\n"
           "      if (" << comparison ("str", "s") << ")\n"
           "        return " << (option[TYPE] ? "wordptr" : "s") << ";\n"
        << If (option[LENTABLE])
        << DecIndent (4)
        << "        }\n"
        << "      lengthptr++;\n"
        << EndIf () // LENTABLE
        << "      wordptr++;\n"
           "    }\n"
           "  return 0;\n"
        << EndIf () // DUP, _total_duplicates
        << "        compare:\n"
        << If (option[TYPE])
        << "          {\n"
           "            ${register} ${const_always} char *s = resword->${SLOT_NAME}"
        << (option[SHAREDLIB] ? " + ${STRINGPOOL_NAME}" : "")
        << ";\n\n"
           "            if (" << comparison ("str", "s") << ")\n"
        << "              return resword;\n"
           "          }\n"
        << Else () // If (!option[TYPE])
        << "          if (" << comparison ("str", "resword") << ")\n"
           "            return resword;\n"
        << Endif () // option[TYPE]
        << "        }\n"
    }
  else
    {
      *this << "      if (key <= ${PREFIX}MAX_HASH_VALUE)\n";
      if (option[DUP])
        {
          *this
            << SetIndent (8)
            << "{\n"
               "  ${register} int index = lookup[key];\n\n"
               "  if (index >= 0)\n"
            << If (option[LENTABLE])
            << "    {\n"
               "      if (len == ${LENGTHTABLE_NAME}[index])\n"
            << IncIndent (4)
            << EndIf () // LENTABLE
            << "    {\n"
               "      ${register} ${const_always} char *s = ${WORDLIST_NAME}[index]"
            << (option[TYPE] ? ".${SLOT_NAME}" : "")
            << (option[SHAREDLIB] ? " + ${STRINGPOOL_NAME}" : "")
            << ";\n\n"
               "      if (" << comparison ("str", "s") << ")\n"
               "        return " << (option[TYPE] ? "&${WORDLIST_NAME}[index]" : "s")
            << ";\n"
            << "    }\n"
            << If (option[LENTABLE])
            << DecIndent (4)
            << "    }\n"
            << EndIf () // LENTABLE

            << If (_total_duplicates > 0)
            << "  else if (index < -${PREFIX}TOTAL_KEYWORDS)\n"
               "    {\n"
               "      ${register} int offset = - 1 - ${PREFIX}TOTAL_KEYWORDS - index;\n"
            << If (option[LENTABLE])
            << "      ${register} ${const_always} " << smallest_integral_type (_max_key_len)
            << " *lengthptr = &${LENGTHTABLE_NAME}[${PREFIX}TOTAL_KEYWORDS + lookup[offset]];\n"
            << Endif () // LENTABLE
            << "      ${register} " << output_const_type (const_readonly_array, _wordlist_eltype)
            << "*wordptr = &${WORDLIST_NAME}[${PREFIX}TOTAL_KEYWORDS + lookup[offset]];\n"
            << "      ${register}" << output_const_type (const_readonly_array, _wordlist_eltype)
            << "*wordendptr = wordptr + -lookup[offset + 1];\n\n"
               "      while (wordptr < wordendptr)\n"
               "        {\n"
            << If (option[LENTABLE])
            << "          if (len == *lengthptr)\n"
               "            {\n"
            << IncIndent (4)
            << Endif () // LENTABLE
            << "          ${register} ${const_always} char *s = "
            << (option[TYPE] ? "wordptr->${SLOT_NAME}" : "*wordptr")
            << (option[SHAREDLIB] ? " + ${STRINGPOOL_NAME}" : "")
            << ";\n\n"
            << "          if (" << comparison ("str", "s") << ")\n"
            << "            return " << (option[TYPE] ? "wordptr" : "s") << ";\n"
            << If (option[LENTABLE])
            << DecIndent (4)
            << "            }\n"
            << Endif ()
            << If (option[LENTABLE])
            << "          lengthptr++;\n"
            << Endif () // LENTABLE
            << "          wordptr++;\n"
               "        }\n"
               "    }\n"
            << EndIf () // _total_duplicates > 0
            << "   }\n"
        }
      else
        {
          *this
            << SetIndent (8)
            << If (option[LENTABLE])
            << "if (len == ${LENGTHTABLE_NAME}[key])\n"
            << Endif () // LENTABLE
            << If (option[SHAREDLIB])
            << If (!option[LENTABLE])
            << "  {\n"
            << "    ${register} int o = ${WORDLIST_NAME}[key]"
            << (option[TYPE] ? ".${SLOT_NAME}" : "")
            << ";\n"
               "    if (o >= 0)\n"
               "      {\n"
            << IncIndent (4)
            << "    ${register} ${const_always} char *s = o"
            << Else () // If (!option[LENTABLE])
                /* No need for the (o >= 0) test, because the
                   (len == lengthtable[key]) test already guarantees that
                   key points to nonempty table entry.  */
            << "  {\n"
               "  ${register} ${const_always} char *s = ${WORDLIST_NAME}[key]"
            << (option[TYPE] ? ".${SLOT_NAME}" : "")
            << EndIf () // LENTABLE
            << " + ${STRINGPOOL_NAME}"
            << Else () // If (!option[SHAREDLIB])
            << "  {\n"
               "  ${register} ${const_always} char *s = ${WORDLIST_NAME}[key]"
            << If (option[TYPE]) << ".${SLOT_NAME}" << EndIf ()
            << EndIf () // SHAREDLIB
            << ";\n\n"
               "  if ("
            << (!option[SHAREDLIB] && option[NULLSTRINGS] ? "s && " : "")
            << comparison ("str", "s")
            << ")\n"
               "  return " << (option[TYPE] ? "&${WORDLIST_NAME}[key]" : "s") << ";\n"
            << If (option[SHAREDLIB] && !option[LENTABLE])
            << DecIndent (4)
            << "  }\n"
            << Endif () // option[SHAREDLIB] && !option[LENTABLE]
            << "}\n";
        }
    }

  *this
    << "    }\n"
       "  return 0;\n";
}
