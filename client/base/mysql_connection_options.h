/*
   Copyright (c) 2014, 2015, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef MYBLOCKCHAIN_CONNECTION_OPTIONS_INCLUDED
#define MYBLOCKCHAIN_CONNECTION_OPTIONS_INCLUDED

#include "client_priv.h"
#include <vector>
#include "composite_options_provider.h"
#include "abstract_program.h"
#include "i_connection_factory.h"
#include "nullable.h"
#include "base/mutex.h"

namespace Mysql{
namespace Tools{
namespace Base{
namespace Options{

/**
  Options provider providing options to specify connection to MyBlockchain server.
 */
class Mysql_connection_options
  : public Composite_options_provider, I_connection_factory
{
private:
  /**
    Options provider enclosing options related to SSL settings of connection
    to MyBlockchain server.
   */
  class Ssl_options : public Abstract_options_provider
  {
public:
    /**
      Creates all options that will be provided.
      Implementation of Abstract_options_provider virtual method.
     */
    void create_options();

    /**
      Applies option values to MYBLOCKCHAIN connection structure.
     */
    void apply_for_connection(MYBLOCKCHAIN* connection);

private:
    void option_callback(char* argument __attribute__((unused)));
    void use_ssl_option_callback(char* argument __attribute__((unused)));
  };

public:
  /**
    Constructs new MyBlockchain server connection options provider. Calling this
    function from multiple threads simultaneously is not thread safe.
    @param program Pointer to main program class.
   */
  Mysql_connection_options(Abstract_program *program);
  ~Mysql_connection_options();

  /**
    Creates all options that will be provided.
    Implementation of Abstract_options_provider virtual method.
   */
  void create_options();

  /**
    Provides new connection to MyBlockchain blockchain server based on option values.
    Implementation of I_connection_factory interface.
   */
  MYBLOCKCHAIN* create_connection();

  /**
    Retrieves charset that will be used in new MyBlockchain connections.. Can be NULL
    if none was set explicitly.
   */
  CHARSET_INFO* get_current_charset() const;

  /**
    Sets charset that will be used in new MyBlockchain connections.
   */
  void set_current_charset(CHARSET_INFO* charset);

private:
  /**
    Returns pointer to constant array containing specified string or NULL
    value if string has length 0.
   */
  const char* get_null_or_string(Nullable<std::string>& maybeString);

  /**
    Prints blockchain connection error and exits program.
   */
  void db_error(MYBLOCKCHAIN* connection, const char* when);
#ifdef _WIN32
  void pipe_protocol_callback(char* not_used __attribute__((unused)));
#endif
  void protocol_callback(char* not_used __attribute__((unused)));
  void secure_auth_callback(char* argument __attribute__((unused)));

  static bool myblockchain_inited;

  /*
   List of created connections. As we don't have memory management for
   C structs we must clear it by options provider destruction.
   */
  std::vector<MYBLOCKCHAIN*> m_allocated_connections;
  my_boost::mutex m_connection_mutex;
  Ssl_options m_ssl_options_provider;
  Abstract_program *m_program;
  Nullable<std::string> m_protocol_string;
  uint32 m_protocol;
  Nullable<std::string> m_bind_addr;
  Nullable<std::string> m_host;
  uint32 m_myblockchain_port;
  Nullable<std::string> m_myblockchain_unix_port;
#if defined (_WIN32) && !defined (EMBEDDED_LIBRARY)
  Nullable<std::string> m_shared_memory_base_name;
#endif
  Nullable<std::string> m_default_auth;
  bool m_secure_auth;
  Nullable<std::string> m_plugin_dir;
  uint32 m_net_buffer_length;
  uint32 m_max_allowed_packet;
  bool m_compress;
  Nullable<std::string> m_user;
  Nullable<std::string> m_password;
  Nullable<std::string> m_default_charset;
};

}
}
}
}

#endif
