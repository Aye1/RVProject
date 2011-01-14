// Implementation of the command line parsing class.
//
// Copyright (C) 2001  Ole Laursen <olau@hardworking.dk>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdlib>		// for exit
#include <algorithm>

#include "commandoptions.h"

// true if val is in container con
template <typename T, typename C>
bool is_in_container(T val, C con)
{
  return std::find(con.begin(), con.end(), val) != con.end();
}


void commandoptions::process_command_line(int argc, const char *argv[])
{
  argument_iterator ait = argument_table.begin();
  std::vector<flag_iterator> processed_flags; 
  
  int i = 1;
  while (i < argc) {
    std::string s(argv[i]);
    
    if (s[0] == '-') {		// option encountered
      
      if (s.size() == 1)	// ensure we got some meat to work on
	throw commandoptions_error("'-' specified without option name");
      
      if (s[1] == '-') {	// long option
	
	if (s.size() == 2)	// ensure we got some meat to work on
	  throw commandoptions_error("'--' specified without option name");

	std::string name = s.substr(2);

	// catch the built-in options
	if (name == "help")
	  print_help(argv[0]);
	
	if (name == "usage")
	  print_usage(argv[0]);
	
	// search flag table
	for (flag_iterator it = flag_table.begin(); it != flag_table.end(); ++it)
	  if (it->long_name == name) {
	    // make sure we don't get "-f -f" = ""
	    if (!is_in_container(it, processed_flags)) {
	      *(it->par) = !(*(it->par)); // reverse flag
	      processed_flags.push_back(it); // remember this flag
	    }
	    
	    goto next_token;
	  }

	// then the option table
	for (option_iterator it = option_table.begin(); it != option_table.end(); ++it)
	  if (it->long_name == name) {
	    if (++i >= argc)	// check that there's one more waiting
	      throw commandoptions_error("option '--" + name + "' specified without argument");
	    else if (!it->par->update(argv[i]))
	      throw commandoptions_error("invalid argument '" + std::string(argv[i]) +
				   "' for option '--" + name + "'");
	    
	    goto next_token;
	  }

	// if we get here, the option wasn't recognized
	throw commandoptions_error("unrecognized option '" + s + "'");
      }
      else {			// short option(s)

	bool has_had_option = false;
	
	// loop through entire string in case some flags were grouped
	unsigned int j = 1;
	while (j < s.length()) {
	  char name = s[j];
	
	  // catch the built-in short help option
	  if (name == '?')
	    print_help(argv[0]);
	
	  // search flag table
	  for (flag_iterator it = flag_table.begin(); it != flag_table.end(); ++it)
	    if (it->short_name == name) {
	      if (!is_in_container(it, processed_flags)) {
		*(it->par) = !(*(it->par)); // reverse flag
		processed_flags.push_back(it); // remember the flag
	      }
	  
	      goto next_option_in_group;
	    }
	  
	  // then the option table
	  for (option_iterator it = option_table.begin(); it != option_table.end(); ++it)
	    if (it->short_name == name) {
	      if (has_had_option)
		throw commandoptions_error("more than one option in the option group '"
					   + s + "' requires an argument");
	      else if (++i >= argc) // ensure that there's one more waiting
		throw commandoptions_error(std::string("option '-") + name + "' specified without argument");
	      else if (!it->par->update(argv[i]))
		throw commandoptions_error("invalid argument '" + std::string(argv[i]) +
					   "' for option '-" + name + "'");
	      has_had_option = true;

	      goto next_option_in_group;
	    }
	  
	  // if we get here, the option wasn't recognized
	  throw commandoptions_error(std::string("unrecognized option '-") + name + "'");

	next_option_in_group:
	  ++j;
	}
      }
    }
    else {			// just a normal argument
      if (ait == argument_table.end())
	throw commandoptions_error("too many arguments specified (last one processed is '" + s + "')");
      else if (!ait->par->update(s))
	throw commandoptions_error("invalid argument '" + s + "' passed as " + requirify(ait->name));
      else
	++ait;			// prepare for next argument
    }
    
  next_token:
    ++i;
  }

  // check that we got enough arguments
  if (ait != argument_table.end()) {
    std::string missing;
    while (ait != argument_table.end()) { // build up list of missing arguments
      missing += " '" + requirify(ait->name) + "'";
      ++ait;
    }
    
    throw commandoptions_error("not enough arguments specified, missing" + missing);
  }
}

// FIXME: switch to dedicated functions for handling encountered options, the
// above function is much too long


std::string commandoptions::strip_path(std::string s)
{
#ifdef HAVE_DOS
  return s.substr(s.rfind('\\') + 1);
#else
  return s.substr(s.rfind('/') + 1);
#endif
}

void commandoptions::print_usage(std::string executable)
{
  // print usage and program name
  std::cerr << "Usage: " << strip_path(executable);

  // gather and print flags
  std::string shorts = "-";	// we sort the flags into short and long ones
  std::string longs;

  for (flag_iterator i = flag_table.begin(); i != flag_table.end(); ++i) {
    if (i->short_name != '\0')
      shorts += i->short_name;
    else
      longs += ' ' + optionalify("--" + i->long_name);
  }

  // insert -? (for help)
  shorts += '?';
    
  std::cerr << ' ' << optionalify(shorts) << longs; 

  
  // print options
  for (option_iterator i = option_table.begin(); i != option_table.end(); ++i)
    if (i->short_name != '\0')
      std::cerr << ' ' << optionalify(std::string("-") + i->short_name  + " " + i->arg_name);
    else
      std::cerr << ' ' << optionalify("--" + i->long_name + " " + i->arg_name);

  // then print --usage so it doesn't clutter up the more interesting options
  std::cerr << ' ' + optionalify("--usage");
  
  
  // and finally print arguments
  for (argument_iterator i = argument_table.begin(); i != argument_table.end(); ++i)
    std::cerr << ' ' << requirify(i->name);

  
  std::cerr << std::endl;
  
  std::exit(1);
}


// helper for securing right indentation
void indent_for_description(int current_indent)
{
  const int description_indentation = 24;

  int extra_space = description_indentation - current_indent;
    
  if (extra_space > 0)
    std::cerr << std::string(extra_space, ' ');
  else
    std::cerr << ' ';		// we always need at least one space
}


void commandoptions::print_help(std::string executable)
{
  // print "Usage: programname [OPTIONS...]"
  std::cerr << "Usage: " << strip_path(executable) << ' ' << optionalify("OPTIONS...");

  // concatenate argument names
  for (argument_iterator i = argument_table.begin(); i != argument_table.end(); ++i)
    std::cerr << ' ' << requirify(i->name);

  std::cerr << std::endl;

  if (flag_table.size() > 0 || option_table.size() > 0)
    std::cerr << "\nOptions:\n";
  
  // then list the flags and options
  for (flag_iterator i = flag_table.begin(); i != flag_table.end(); ++i) {
    std::cerr << "  ";		// indentation

    int x = 2;			// we need to keep track of how far we've reached
    
    if (i->short_name != '\0') {
      std::cerr << '-' << i->short_name;
      x += 2;
    }

    if (i->long_name.length() > 0) {
      if (i->short_name != '\0') { 
	std::cerr << ", ";	// add separator
	x += 2;
      }
      
      std::cerr << "--" << i->long_name;
      
      x += 2 + i->long_name.length();
    }
    
    indent_for_description(x);

    std::cerr << i->des << std::endl;
  }

  for (option_iterator i = option_table.begin(); i != option_table.end(); ++i) {
    std::cerr << "  ";		

    int x = 2;			
    
    if (i->short_name != '\0') {
      std::cerr << '-' << i->short_name;
      x += 2;
    }

    if (i->long_name.length() > 0) {
      if (i->short_name != '\0') { // output separator
	std::cerr << ", ";	
	x += 2;
      }
      
      std::cerr << "--" << i->long_name;
      
      x += 2 + i->long_name.length();
    }

    // output the name of the option argument
    std::cerr << ' ' << i->arg_name;

    x += 1 + i->arg_name.length();

    indent_for_description(x);
      
    std::cerr << i->des << std::endl;
  }


  bool first_argument = true;

  // output descriptions of arguments, if any
  for (argument_iterator i = argument_table.begin(); i != argument_table.end(); ++i) {
    if (i->des.length() > 0) {
      if (first_argument)	// prefix the first argument
	std::cerr << "\nArguments:\n";
      first_argument = false;

      std::cerr << "  " << i->name;

      indent_for_description(2 + i->name.length());
      std::cerr << i->des << std::endl;
    }
  }

  std::cerr << "\nHelp options:\n";
  
  // and finally output help about help options
  std::string s = "  -h, --help";
  std::cerr << s;
  indent_for_description(s.length());
  std::cerr << "Show this help message\n";

  s = "  --usage";
  std::cerr << s;
  indent_for_description(s.length());
  std::cerr << "Display brief usage message\n";
  
  std::exit(1);
} 
