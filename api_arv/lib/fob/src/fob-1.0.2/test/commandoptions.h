// A -*- C++ -*- class for parsing command line options.
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


#ifndef COMMANDOPTIONS_HEADER
#define COMMANDOPTIONS_HEADER

#include <vector>
#include <string>
#include <exception>
#include <sstream>

class commandoptions
{
public:
  // register an option with pointer to variable, long and short argument names, a
  // description of the option and the name of the argument to the option
  template <typename T>
  void register_option(T &par, std::string long_name, char short_name,
		       std::string des, std::string arg_name)
  {
    option_table.push_back(option(new updater<T>(par), long_name, short_name, des, arg_name));
  }

  void register_flag(bool &par, std::string long_name, char short_name, std::string des)
  {
    flag_table.push_back(flag(par, long_name, short_name, des));
  }
  
  template <typename T>
  void register_argument(T &par, std::string name, std::string des)
  {
    argument_table.push_back(argument(new updater<T>(par), name, des));
  }

  void process_command_line(int argc, const char *argv[]);

  ~commandoptions()
  {
    // make sure dynamically allocated stuff is cleaned up
    for (std::vector<updaterbase *>::iterator it = clean_up_table.begin();
	 it != clean_up_table.end(); ++it)
      delete (*it);
  }
  
  // private data structures
  // first a few classes for updating a variable from a stream
  class updaterbase 
  {
  public:
    virtual bool update(std::string s) = 0;
  };
  

  
private:
  // member helpers
    
  std::string requirify(std::string s) 
  {
    return '<' + s + '>';
  }
  std::string optionalify(std::string s) 
  {
    return '[' + s + ']';
  }

  std::string strip_path(std::string s);

  void print_usage(std::string executable);
  void print_help(std::string executable);
  
  template <typename T>
  class updater: public updaterbase
  {
  public:
    updater(T &par):
      var(par)
    {}

    // update the contained variable through reference - return false upon failure
    virtual bool update(std::string s)
    {
      std::istringstream is(s);
      is >> var;
      if (!is)
	return false;
      else
	return true;
    }
    
  private:
    T &var;
  };

  // a structure for keeping a command line option
  struct option 
  {
    updaterbase *par;
    std::string long_name, des, arg_name;
    char short_name;

    option(updaterbase *p, std::string ln, char sn, std::string d, std::string an):
      par(p), long_name(ln), des(d), arg_name(an), short_name(sn)
    {}
    option(const option &o):
      par(o.par), long_name(o.long_name), des(o.des), arg_name(o.arg_name), short_name(o.short_name)
    {}
  };

  // one for keeping a flag
  struct flag
  {
    bool *par;
    std::string long_name, des;
    char short_name;
    
    flag(bool &p, std::string ln, char sn, std::string d):
      par(&p), long_name(ln), des(d), short_name(sn)
    {}
  };
  
  // and one for keeping an argument
  struct argument
  {
    updaterbase *par;
    std::string name, des;
    
    argument(updaterbase *p, std::string n, std::string d):
      par(p), name(n), des(d)
    {}
    argument(const argument &a):
      par(a.par), name(a.name), des(a.des)
    {}
  };

  std::vector<option> option_table;
  std::vector<flag> flag_table;
  std::vector<argument> argument_table;
  typedef std::vector<option>::iterator option_iterator;
  typedef std::vector<flag>::iterator flag_iterator;
  typedef std::vector<argument>::iterator argument_iterator;

  std::vector<updaterbase *> clean_up_table;
};

template <> class commandoptions::updater<std::string>: public commandoptions::updaterbase
{
public:
  updater(std::string &par):
    var(par)
  {}

  // update the contained variable through reference - return false upon failure
  virtual bool update(std::string s)
  {
    var = s;
    return true;
  }
    
private:
	std::string &var;
};
  

// an exception class
class commandoptions_error: std::exception
{
public:
  commandoptions_error(std::string s) throw(): msg(s)
  {}
  ~commandoptions_error() throw()
  {}
    
  virtual const char *what() throw()
  {
    return msg.c_str();
  }
  
private:
  std::string msg;
};


#endif // COMMANDOPTIONS_HEADER
