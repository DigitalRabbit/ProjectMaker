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

/*! \brief  AndroidManifest 用 Parser クラス
 *
 * xmlpp::DomParser を使用して AndroidManifest.xml の内容を取得することに特化したクラスです。\n
 * ベースとなる Package name, Version name, Version code に関しては、\n
 * コンストラクタの時点で読み込みを行います。
 *
 * \author  Digital Rabbit
 * \since   2012/02/22
 */
class ManifestParser
{
public:
    /*! \brief コンストラクタ */
    ManifestParser( const Glib::ustring aFilePath );
    /*! \brief デストラクタ */
    virtual ~ManifestParser();

    /*! \brief アプリケーション名取得 */
    const Glib::ustring getApplicationName();
    /*! \brief Version name 取得 */
    const Glib::ustring getVersionName();
    /*! \brief Version code 取得 */
    const Glib::ustring getVersionCode();

    /*! \brief DOM root node 取得 */
    const xmlpp::Node* getRootNode();

    void print_node( const xmlpp::Node*, unsigned int = 0 );
    void print_indentation( unsigned int );

protected:

private:
    /*! \brief DOM XML parser */
    xmlpp::DomParser mParser;
    /*! \brief &lt;package&gt;属性値 */
    Glib::ustring mAppName;
    /*! \brief &lt;versionName&gt;属性値 */
    Glib::ustring mVerName;
    /*! \brief &lt;versionCode&gt;属性値 */
    Glib::ustring mVerCode;

};

} }   // End of Digirabi::Android

