/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
#include "stdafx.h"
#include "tinyxml.h"

// The goal of the seperate error file is to make the first
// step towards localization. tinyxml (currently) only supports
// english error messages, but the could now be translated.
//
// It also cleans up the code a bit.
//

const TCHAR* TiXmlBase::errorString[ TIXML_ERROR_STRING_COUNT ] =
{
	_UNI("No error"),
	_UNI("Error"),
	_UNI("Failed to open file"),
	_UNI("Error parsing Element."),
	_UNI("Failed to read Element name"),
	_UNI("Error reading Element value."),
	_UNI("Error reading Attributes."),
	_UNI("Error: empty tag."),
	_UNI("Error reading end tag."),
	_UNI("Error parsing Unknown."),
	_UNI("Error parsing Comment."),
	_UNI("Error parsing Declaration."),
	_UNI("Error document empty."),
	_UNI("Error null (0) or unexpected EOF found in input stream."),
	_UNI("Error parsing CDATA."),
	_UNI("Error when TiXmlDocument added to document, because TiXmlDocument can only be at the root."),
};
