#!/usr/bin/env python
#############################################################################
##
## Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
## All rights reserved.
## Contact: Nokia Corporation (qt-info@nokia.com)
##
## This file is part of the test suite of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:LGPL$
## GNU Lesser General Public License Usage
## This file may be used under the terms of the GNU Lesser General Public
## License version 2.1 as published by the Free Software Foundation and
## appearing in the file LICENSE.LGPL included in the packaging of this
## file. Please review the following information to ensure the GNU Lesser
## General Public License version 2.1 requirements will be met:
## http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## In addition, as a special exception, Nokia gives you certain additional
## rights. These rights are described in the Nokia Qt LGPL Exception
## version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU General
## Public License version 3.0 as published by the Free Software Foundation
## and appearing in the file LICENSE.GPL included in the packaging of this
## file. Please review the following information to ensure the GNU General
## Public License version 3.0 requirements will be met:
## http://www.gnu.org/copyleft/gpl.html.
##
## Other Usage
## Alternatively, this file may be used in accordance with the terms and
## conditions contained in a signed written agreement between you and Nokia.
##
##
##
##
##
## $QT_END_LICENSE$
##
#############################################################################

import sys
import os
import xml.dom.minidom

doc_cache = {}

class DraftResolution:
    # See http://www.unicode.org/cldr/process.html for description
    unconfirmed = 'unconfirmed'
    provisional = 'provisional'
    contributed = 'contributed'
    approved = 'approved'
    _values = { unconfirmed : 1, provisional : 2, contributed : 3, approved : 4 }
    def __init__(self, resolution):
        self.resolution = resolution
    def toInt(self):
        return DraftResolution._values[self.resolution]

class Error:
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return self.msg

def findChild(parent, tag_name, arg_name=None, arg_value=None, draft=None):
    for node in parent.childNodes:
        if node.nodeType != node.ELEMENT_NODE:
            continue
        if node.nodeName != tag_name:
            continue
        if arg_value:
            if not node.attributes.has_key(arg_name):
                continue
            if node.attributes[arg_name].nodeValue != arg_value:
                continue
        if draft:
            if not node.attributes.has_key('draft'):
                # if draft is not specified then it's approved
                return node
            value = node.attributes['draft'].nodeValue
            value = DraftResolution(value).toInt()
            exemplar = DraftResolution(draft).toInt()
            if exemplar > value:
                continue
        return node
    return False

def _findEntryInFile(file, path, draft=None, attribute=None):
    doc = False
    if doc_cache.has_key(file):
        doc = doc_cache[file]
    else:
        doc = xml.dom.minidom.parse(file)
        doc_cache[file] = doc

    elt = doc.documentElement
    tag_spec_list = path.split("/")
    last_entry = None
    for i in range(len(tag_spec_list)):
        tag_spec = tag_spec_list[i]
        tag_name = tag_spec
        arg_name = 'type'
        arg_value = ''
        left_bracket = tag_spec.find('[')
        if left_bracket != -1:
            tag_name = tag_spec[:left_bracket]
            arg_value = tag_spec[left_bracket+1:-1].split("=")
            if len(arg_value) == 2:
                arg_name = arg_value[0]
                arg_value = arg_value[1]
            else:
                arg_value = arg_value[0]
        alias = findChild(elt, 'alias')
        if alias and alias.attributes['source'].nodeValue == 'locale':
            path = alias.attributes['path'].nodeValue
            aliaspath = tag_spec_list[:i] + path.split("/")
            def resolve(x, y):
                if y == '..':
                    return x[:-1]
                return x + [y]
            # resolve all dot-dot parts of the path
            aliaspath = reduce(resolve, aliaspath, [])
            # remove attribute specification that our xpathlite doesnt support
            aliaspath = map(lambda x: x.replace("@type=", "").replace("'", ""), aliaspath)
            # append the remaining path
            aliaspath = aliaspath + tag_spec_list[i:]
            aliaspath = "/".join(aliaspath)
            # "locale" aliases are special - we need to start lookup from scratch
            return (None, aliaspath)
        elt = findChild(elt, tag_name, arg_name, arg_value, draft)
        if not elt:
            return ("", None)
    if attribute is not None:
        if elt.attributes.has_key(attribute):
            return (elt.attributes[attribute].nodeValue, None)
        return (None, None)
    return (elt.firstChild.nodeValue, None)

def findAlias(file):
    if not doc_cache.has_key(file):
        return False
    doc = doc_cache[file]
    alias_elt = findChild(doc.documentElement, "alias")
    if not alias_elt:
        return False
    if not alias_elt.attributes.has_key('source'):
        return False
    return alias_elt.attributes['source'].nodeValue

def _findEntry(base, path, draft=None, attribute=None):
    file = base
    if base.endswith(".xml"):
        filename = base
        base = base[:-4]
    else:
        file = base + ".xml"
    (dirname, filename) = os.path.split(base)
    items = filename.split("_")
    # split locale name into items and iterate through them from back to front
    # example: az_Latn_AZ => [az_Latn_AZ, az_Latn, az]
    items = reversed(map(lambda x: "_".join(items[:x+1]), range(len(items))))
    for item in items:
        file = dirname + "/" + item + ".xml"
        if os.path.isfile(file):
            alias = findAlias(file)
            if alias:
                # if alias is found we should follow it and stop processing current file
                # see http://www.unicode.org/reports/tr35/#Common_Elements
                aliasfile = os.path.dirname(file) + "/" + alias + ".xml"
                if not os.path.isfile(aliasfile):
                    raise Error("findEntry: fatal error: found an alias '%s' to '%s', but the alias file couldnt be found" % (filename, alias))
                # found an alias, recurse into parsing it
                result = _findEntry(aliasfile, path, draft, attribute)
                return result
            (result, aliaspath) = _findEntryInFile(file, path, draft, attribute)
            if aliaspath:
                # start lookup again because of the alias source="locale"
                return _findEntry(base, aliaspath, draft, attribute)
            if result:
                return result
    return None

def findEntry(base, path, draft=None, attribute=None):
    file = base
    if base.endswith(".xml"):
        file = base
        base = base[:-4]
    else:
        file = base + ".xml"
    (dirname, filename) = os.path.split(base)

    result = None
    while path:
        result = _findEntry(base, path, draft, attribute)
        if result:
            return result
        (result, aliaspath) = _findEntryInFile(dirname + "/root.xml", path, draft, attribute)
        if result:
            return result
        if not aliaspath:
            raise Error("findEntry: fatal error: %s: did not found key %s" % (filename, path))
        path = aliaspath

    return result

