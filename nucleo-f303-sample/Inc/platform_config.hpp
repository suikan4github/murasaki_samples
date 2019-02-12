/**
 * \file platform_config.hpp
 *
 * \date 2018/01/07
 * \author takemasa
 * \brief Application dependent configuration.
 * \details
 * If you want to override the macro definition inside platform_config.hpp,
 * add your definition here.
 */

#ifndef PLATFORM_CONFIG_HPP_
#define PLATFORM_CONFIG_HPP_

/*
 * If you want to override the configuration value in the platform_config.hpp,
 * define the MACRO here.
 */

// E X A M P L E
// Un-comment the folliwing line will override the value definition inside platform_config.hpp
// #define PLATFORM_CONFIG_DEBUG_LINE_SIZE  128

// Enforce printf buffer smaler
#define PLATFORM_CONFIG_DEBUG_BUFFER_SIZE 1024

// Define following macro as true to disable the assertion inside murasaki class library.
#define MURASAKI_CONFIG_NODEBUG false

// Define following macro as true to disable the Syslog inside murasaki class library
#define MURASAKI_CONFIG_NOSYSLOG false

#endif /* PLATFORM_CONFIG_HPP_ */
