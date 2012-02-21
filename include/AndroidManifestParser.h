/*-************************************************************
 *  ____            _        _   ____       _     _       _
 * |  _ \(_) __ _(_) |_ __ _| | |  _ \ __ _| |__ | |__ (_) |_
 * | | | | |/ _` | | __/ _` | | | |_) / _` | '_ \| '_ \| | __|
 * | |_| | | (_| | | |_ (_| | | |  _ < (_| | |_) | |_) | | |_
 * |____/|_|\__, |_|\__\__,_|_| |_| \_\__,_|_.__/|_.__/|_|\__|
 *          |___/
 **************************************************************
 * Copyright(C) 2012 Digital Rabbit All Rights Reserved.  *//*!
 *
 * \file    AndroidManifestParser.h
 *
 * \author  Digital Rabbit(digital.rabbit.jp@gmail.com)
 * \date    2012/02/22
 */
#pragma once

#include <glibmm/ustring.h>
#include <libxml++/parsers/domparser.h>

namespace digirabi {
    namespace android {

class ManifestParser
{
public:
    ManifestParser( const Glib::ustring );
    virtual ~ManifestParser();

    void print_node( const xmlpp::Node*, unsigned int = 0 );
    void print_indentation( unsigned int );

    const Glib::ustring getApplicationName();
    const Glib::ustring getVersionName();
    const Glib::ustring getVersionCode();

    const xmlpp::Node* getRootNode();

protected:

private:
    xmlpp::DomParser mParser;
    Glib::ustring mAppName;
    Glib::ustring mVerName;
    Glib::ustring mVerCode;

};

} }   // End of Digirabi::Android

