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
 * \file    AndroidManifestParser.cc
 *
 * \author  Digital Rabbit(digital.rabbit.jp@gmail.com)
 * \date    2012/02/22
 */

#include <AndroidManifestParser.h>

#include <iostream>
#include <libxml++/document.h>
#include <libxml++/nodes/commentnode.h>
#include <libxml++/nodes/textnode.h>
#include <libxml++/nodes/contentnode.h>
#include <libxml++/nodes/element.h>

namespace digirabi {
    namespace android {

ManifestParser::ManifestParser( const Glib::ustring aFilePath )
{
    mParser.set_substitute_entities();
    mParser.parse_file( aFilePath );

    const xmlpp::Node* node = getRootNode();
    Glib::ustring nodeName = node->get_name();
    // Recurse through child nodes:
    xmlpp::Node::NodeList list = node->get_children();
    std::cout << nodeName << std::endl;
    for( xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end() && ( nodeName != "manifest" ); ++iter )
    {
        node = *iter;
        nodeName = node->get_name();
    }

    const xmlpp::Element* element = dynamic_cast< const xmlpp::Element* >( node );
    if( element == nullptr )    return;

    //Print attributes:
    const xmlpp::Element::AttributeList& attributes = element->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
        const xmlpp::Attribute* attribute = *iter;

        if( attribute->get_name() == "package" )
            mAppName = "Package name : " + attribute->get_value();
        else if( attribute->get_name() == "versionName" )
            mVerName = "Version name : " + attribute->get_value();
        else if( attribute->get_name() == "versionCode" )
            mVerCode = "Version code : " + attribute->get_value();
    }

    if( mAppName.empty() )
    {
        mAppName = "This directory is not Android project.";
    }

    return;
}

ManifestParser::~ManifestParser() {}

const Glib::ustring ManifestParser::getApplicationName()
{
    return ( mAppName );
}

const Glib::ustring ManifestParser::getVersionName()
{
    return ( mVerName );
}

const Glib::ustring ManifestParser::getVersionCode()
{
    return ( mVerCode );
}

const xmlpp::Node* ManifestParser::getRootNode()
{
    return ( mParser.get_document()->get_root_node() ); // deleted by DomParser.
}

void ManifestParser::print_node( const xmlpp::Node* node, unsigned int indentation )
{

    std::cout << std::endl; //Separate nodes by an empty line.

    const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
    const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
    const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

    if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
        return;

    const Glib::ustring nodename = node->get_name();

    if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
    {
        print_indentation(indentation);

        const Glib::ustring namespace_prefix = node->get_namespace_prefix();
        if(namespace_prefix.empty())
            std::cout << "Node name = " << nodename << std::endl;
        else
            std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
    }
    else if(nodeText) //Let's say when it's text. - e.g. let's say what that white space is.
    {
        print_indentation(indentation);
        std::cout << "Text Node" << std::endl;
    }

    //Treat the various node types differently: 
    if(nodeText)
    {
        print_indentation(indentation);
        std::cout << "text = \"" << nodeText->get_content() << "\"" << std::endl;
    }
    else if(nodeComment)
    {
        print_indentation(indentation);
        std::cout << "comment = " << nodeComment->get_content() << std::endl;
    }
    else if(nodeContent)
    {
        print_indentation(indentation);
        std::cout << "content = " << nodeContent->get_content() << std::endl;
    }
    else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
    {
        //A normal Element node:

        //line() works only for ElementNodes.
        print_indentation(indentation);
        std::cout << "     line = " << node->get_line() << std::endl;

        //Print attributes:
        const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
        for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
        {
            const xmlpp::Attribute* attribute = *iter;
            print_indentation(indentation);

            const Glib::ustring namespace_prefix = attribute->get_namespace_prefix();
            if(namespace_prefix.empty())
                std::cout << "  Attribute " << attribute->get_name() << " = " << attribute->get_value() << std::endl; 
            else
                std::cout << "  Attribute " << namespace_prefix  << ":" << attribute->get_name() << " = " << attribute->get_value() << std::endl;
        }

        const xmlpp::Attribute* attribute = nodeElement->get_attribute("title");
        if(attribute)
        {
            std::cout << "title found: =" << attribute->get_value() << std::endl;
        }
    }

    if(!nodeContent)
    {
        //Recurse through child nodes:
        xmlpp::Node::NodeList list = node->get_children();
        for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
           print_node(*iter, indentation + 2); //recursive
        }
    }
}

void ManifestParser::print_indentation(unsigned int indentation)
{
    for(unsigned int i = 0; i < indentation; ++i)
        std::cout << " ";
}

} }   // End of Digirabi::Android

