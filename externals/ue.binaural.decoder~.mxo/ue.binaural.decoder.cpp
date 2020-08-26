/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2015"
license: "GPL)"
name: "Binaural decoder"
version: "1.0"
Code generated with Faust 2.26.2 (https://faust.grame.fr)
Compilation options: -lang cpp -double -ftz 0
------------------------------------------------------------ */

#ifndef  __ue_binaural_decoder_H__
#define  __ue_binaural_decoder_H__

/************************************************************************

    IMPORTANT NOTE : this file contains two clearly delimited sections :
    the ARCHITECTURE section (in two parts) and the USER section. Each section
    is governed by its own copyright and license. Please check individually
    each section for license and copyright information.
*************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
    FAUST Architecture File
    Copyright (C) 2004-2020 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.

    MAX MSP SDK : in order to compile a MaxMSP external with this
    architecture file you will need the official MaxMSP SDK from
    cycling'74. Please check the corresponding license.

 ************************************************************************
 ************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <unistd.h>
#endif

#ifdef WIN32
#ifndef NAN
    static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
    #define NAN (*(const float *) __nan)
#endif
#endif

// FAUSTFLOAT is setup by faust2max6

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN SimpleParser.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

// ---------------------------------------------------------------------
//                          Simple Parser
// A parser returns true if it was able to parse what it is
// supposed to parse and advance the pointer. Otherwise it returns false
// and the pointer is not advanced so that another parser can be tried.
// ---------------------------------------------------------------------

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctype.h>

#ifndef _WIN32
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

struct itemInfo {
    std::string type;
    std::string label;
    std::string url;
    std::string address;
    int index;
    double init;
    double min;
    double max;
    double step;
    std::vector<std::pair<std::string, std::string> > meta;
    
    itemInfo():index(0), init(0.), min(0.), max(0.), step(0.)
    {}
};

// ---------------------------------------------------------------------
//                          Elementary parsers
// ---------------------------------------------------------------------

// Report a parsing error
static bool parseError(const char*& p, const char* errmsg)
{
    std::cerr << "Parse error : " << errmsg << " here : " << p << std::endl;
    return true;
}

/**
 * @brief skipBlank : advance pointer p to the first non blank character
 * @param p the string to parse, then the remaining string
 */
static void skipBlank(const char*& p)
{
    while (isspace(*p)) { p++; }
}

// Parse character x, but don't report error if fails
static bool tryChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseChar : parse a specific character x
 * @param p the string to parse, then the remaining string
 * @param x the character to recognize
 * @return true if x was found at the begin of p
 */
static bool parseChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseWord : parse a specific string w
 * @param p the string to parse, then the remaining string
 * @param w the string to recognize
 * @return true if string w was found at the begin of p
 */
static bool parseWord(const char*& p, const char* w)
{
    skipBlank(p);
    const char* saved = p;  // to restore position if we fail
    while ((*w == *p) && (*w)) {++w; ++p;}
    if (*w) {
        p = saved;
        return false;
    } else {
        return true;
    }
}

/**
 * @brief parseDouble : parse number [s]dddd[.dddd] and store the result in x
 * @param p the string to parse, then the remaining string
 * @param x the float number found if any
 * @return true if a float number was found at the begin of p
 */
static bool parseDouble(const char*& p, double& x)
{
    std::stringstream reader(p);
    std::streambuf* pbuf = reader.rdbuf();
    
    // Keep position before parsing
    std::streamsize size1 = pbuf->in_avail();
    
    // Parse the number
    reader >> x;
    
    // Keep position after parsing
    std::streamsize size2 = pbuf->in_avail();
    
    // Move from the actual size
    p += (size1 - size2);
    
    // True if the number contains at least one digit
    return (size1 > size2);
}

/**
 * @brief parseString, parse an arbitrary quoted string q...q and store the result in s
 * @param p the string to parse, then the remaining string
 * @param quote the character used to quote the string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseString(const char*& p, char quote, std::string& s)
{
    std::string str;
    skipBlank(p);
    
    const char* saved = p;  // to restore position if we fail
    if (*p++ == quote) {
        while ((*p != 0) && (*p != quote)) {
            str += *p++;
        }
        if (*p++ == quote) {
            s = str;
            return true;
        }
    }
    p = saved;
    return false;
}

/**
 * @brief parseSQString, parse a single quoted string '...' and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseSQString(const char*& p, std::string& s)
{
    return parseString(p, '\'', s);
}

/**
 * @brief parseDQString, parse a double quoted string "..." and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseDQString(const char*& p, std::string& s)
{
    return parseString(p, '"', s);
}

// ---------------------------------------------------------------------
//
//                          IMPLEMENTATION
// 
// ---------------------------------------------------------------------

/**
 * @brief parseMenuItem, parse a menu item ...'low':440.0...
 * @param p the string to parse, then the remaining string
 * @param name the name found
 * @param value the value found
 * @return true if a nemu item was found
 */
static bool parseMenuItem(const char*& p, std::string& name, double& value)
{
    const char* saved = p;  // to restore position if we fail
    if (parseSQString(p, name) && parseChar(p, ':') && parseDouble(p, value)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

static bool parseMenuItem2(const char*& p, std::string& name)
{
    const char* saved = p;  // to restore position if we fail
    // single quoted
    if (parseSQString(p, name)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

/**
 * @brief parseMenuList, parse a menu list {'low' : 440.0; 'mid' : 880.0; 'hi' : 1760.0}...
 * @param p the string to parse, then the remaining string
 * @param names the vector of names found
 * @param values the vector of values found
 * @return true if a menu list was found
 */
static bool parseMenuList(const char*& p, std::vector<std::string>& names, std::vector<double>& values)
{
    std::vector<std::string> tmpnames;
    std::vector<double> tmpvalues;
    const char* saved = p; // to restore position if we fail

    if (parseChar(p, '{')) {
        do {
            std::string n;
            double v;
            if (parseMenuItem(p, n, v)) {
                tmpnames.push_back(n);
                tmpvalues.push_back(v);
            } else {
                p = saved;
                return false;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            values = tmpvalues;
            return true;
        }
    }
    p = saved;
    return false;
}

static bool parseMenuList2(const char*& p, std::vector<std::string>& names, bool debug)
{
    std::vector<std::string> tmpnames;
    const char* saved = p;  // to restore position if we fail
    
    if (parseChar(p, '{')) {
        do {
            std::string n;
            if (parseMenuItem2(p, n)) {
                tmpnames.push_back(n);
            } else {
                goto error;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            return true;
        }
    }
    
error:
    if (debug) { std::cerr << "parseMenuList2 : (" << saved << ") is not a valid list !\n"; }
    p = saved;
    return false;
}

/// ---------------------------------------------------------------------
// Parse list of strings
/// ---------------------------------------------------------------------
static bool parseList(const char*& p, std::vector<std::string>& items)
{
    const char* saved = p;  // to restore position if we fail
    if (parseChar(p, '[')) {
        do {
            std::string item;
            if (!parseDQString(p, item)) {
                p = saved;
                return false;
            }
            items.push_back(item);
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseMetaData(const char*& p, std::map<std::string, std::string>& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas[metaKey] = metaValue;
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseItemMetaData(const char*& p, std::vector<std::pair<std::string, std::string> >& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas.push_back(std::make_pair(metaKey, metaValue));
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse metadatas of the interface:
// "name" : "...", "inputs" : "...", "outputs" : "...", ...
// and store the result as key/value
/// ---------------------------------------------------------------------
static bool parseGlobalMetaData(const char*& p, std::string& key, std::string& value, double& dbl, std::map<std::string, std::string>& metadatas, std::vector<std::string>& items)
{
    const char* saved = p; // to restore position if we fail
    if (parseDQString(p, key)) {
        if (key == "meta") {
            return parseMetaData(p, metadatas);
        } else {
            return parseChar(p, ':') && (parseDQString(p, value) || parseList(p, items) || parseDouble(p, dbl));
        }
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse gui:
// "type" : "...", "label" : "...", "address" : "...", ...
// and store the result in uiItems Vector
/// ---------------------------------------------------------------------
static bool parseUI(const char*& p, std::vector<itemInfo>& uiItems, int& numItems)
{
    const char* saved = p; // to restore position if we fail
    if (parseChar(p, '{')) {
   
        std::string label;
        std::string value;
        double dbl = 0;
        
        do {
            if (parseDQString(p, label)) {
                if (label == "type") {
                    if (uiItems.size() != 0) {
                        numItems++;
                    }
                    if (parseChar(p, ':') && parseDQString(p, value)) {   
                        itemInfo item;
                        item.type = value;
                        uiItems.push_back(item);
                    }
                }
                
                else if (label == "label") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].label = value;
                    }
                }
                
                else if (label == "url") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].url = value;
                    }
                }
                
                else if (label == "address") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].address = value;
                    }
                }
                
                else if (label == "index") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].index = int(dbl);
                    }
                }
                
                else if (label == "meta") {
                    if (!parseItemMetaData(p, uiItems[numItems].meta)) {
                        return false;
                    }
                }
                
                else if (label == "init") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].init = dbl;
                    }
                }
                
                else if (label == "min") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].min = dbl;
                    }
                }
                
                else if (label == "max") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].max = dbl;
                    }
                }
                
                else if (label == "step") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].step = dbl;
                    }
                }
                
                else if (label == "items") {
                    if (parseChar(p, ':') && parseChar(p, '[')) {
                        do {
                            if (!parseUI(p, uiItems, numItems)) {
                                p = saved;
                                return false;
                            }
                        } while (tryChar(p, ','));
                        if (parseChar(p, ']')) {
                            itemInfo item;
                            item.type = "close";
                            uiItems.push_back(item);
                            numItems++;
                        }
                    }
                }
            } else {
                p = saved;
                return false;
            }
            
        } while (tryChar(p, ','));
    
        return parseChar(p, '}');
    } else {
        return true; // "items": [] is valid
    }
}

// ---------------------------------------------------------------------
// Parse full JSON record describing a JSON/Faust interface :
// {"metadatas": "...", "ui": [{ "type": "...", "label": "...", "items": [...], "address": "...","init": "...", "min": "...", "max": "...","step": "..."}]}
//
// and store the result in map Metadatas and vector containing the items of the interface. Returns true if parsing was successfull.
/// ---------------------------------------------------------------------
static bool parseJson(const char*& p,
                      std::map<std::string, std::pair<std::string, double> >& metaDatas0,
                      std::map<std::string, std::string>& metaDatas1,
                      std::map<std::string, std::vector<std::string> >& metaDatas2,
                      std::vector<itemInfo>& uiItems)
{
    parseChar(p, '{');
    
    do {
        std::string key;
        std::string value;
        double dbl = 0;
        std::vector<std::string> items;
        if (parseGlobalMetaData(p, key, value, dbl, metaDatas1, items)) {
            if (key != "meta") {
                // keep "name", "inputs", "outputs" key/value pairs
                if (items.size() > 0) {
                    metaDatas2[key] = items;
                    items.clear();
                } else if (value != "") {
                    metaDatas0[key].first = value;
                } else {
                    metaDatas0[key].second = dbl;
                }
            }
        } else if (key == "ui") {
            int numItems = 0;
            parseChar(p, '[') && parseUI(p, uiItems, numItems);
        }
    } while (tryChar(p, ','));
    
    return parseChar(p, '}');
}

#endif // SIMPLEPARSER_H
/**************************  END  SimpleParser.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/
/************************** BEGIN dsp-combiner.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_combiner__
#define __dsp_combiner__

#include <string.h>
#include <string>
#include <assert.h>
#include <sstream>

/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

// Base class and common code for binary combiners

class dsp_binary_combiner : public dsp {

    protected:

        dsp* fDSP1;
        dsp* fDSP2;

        void buildUserInterfaceAux(UI* ui_interface, const char* name)
        {
            ui_interface->openTabBox(name);
            ui_interface->openVerticalBox("DSP1");
            fDSP1->buildUserInterface(ui_interface);
            ui_interface->closeBox();
            ui_interface->openVerticalBox("DSP2");
            fDSP2->buildUserInterface(ui_interface);
            ui_interface->closeBox();
            ui_interface->closeBox();
        }

        FAUSTFLOAT** allocateChannels(int num, int buffer_size)
        {
            FAUSTFLOAT** channels = new FAUSTFLOAT*[num];
            for (int chan = 0; chan < num; chan++) {
                channels[chan] = new FAUSTFLOAT[buffer_size];
                memset(channels[chan], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
            return channels;
        }

        void deleteChannels(FAUSTFLOAT** channels, int num)
        {
            for (int chan = 0; chan < num; chan++) {
                delete [] channels[chan];
            }
            delete [] channels;
        }

     public:

        dsp_binary_combiner(dsp* dsp1, dsp* dsp2):fDSP1(dsp1), fDSP2(dsp2)
        {}

        virtual ~dsp_binary_combiner()
        {
            delete fDSP1;
            delete fDSP2;
        }

        virtual int getSampleRate()
        {
            return fDSP1->getSampleRate();
        }
        virtual void init(int sample_rate)
        {
            fDSP1->init(sample_rate);
            fDSP2->init(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            fDSP1->instanceInit(sample_rate);
            fDSP2->instanceInit(sample_rate);
        }
        virtual void instanceConstants(int sample_rate)
        {
            fDSP1->instanceConstants(sample_rate);
            fDSP2->instanceConstants(sample_rate);
        }

        virtual void instanceResetUserInterface()
        {
            fDSP1->instanceResetUserInterface();
            fDSP2->instanceResetUserInterface();
        }

        virtual void instanceClear()
        {
            fDSP1->instanceClear();
            fDSP2->instanceClear();
        }

        virtual void metadata(Meta* m)
        {
            fDSP1->metadata(m);
            fDSP2->metadata(m);
        }

};

// Combine two 'compatible' DSP in sequence

class dsp_sequencer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;

    public:

        dsp_sequencer(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
        }

        virtual ~dsp_sequencer()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Sequencer");
        }

        virtual dsp* clone()
        {
            return new dsp_sequencer(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);
            fDSP2->compute(count, fDSP1Outputs, outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

// Combine two DSP in parallel

class dsp_parallelizer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_parallelizer(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
            fDSP2Outputs = new FAUSTFLOAT*[fDSP2->getNumOutputs()];
        }

        virtual ~dsp_parallelizer()
        {
            delete [] fDSP2Inputs;
            delete [] fDSP2Outputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() + fDSP2->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs() + fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Parallelizer");
        }

        virtual dsp* clone()
        {
            return new dsp_parallelizer(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, outputs);

            // Shift inputs/outputs channels for fDSP2
            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                fDSP2Inputs[chan] = inputs[fDSP1->getNumInputs() + chan];
            }
            for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                fDSP2Outputs[chan] = outputs[fDSP1->getNumOutputs() + chan];
            }

            fDSP2->compute(count, fDSP2Inputs, fDSP2Outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

// Combine two 'compatible' DSP in splitter

class dsp_splitter : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

    public:

        dsp_splitter(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_splitter()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Splitter");
        }

        virtual dsp* clone()
        {
            return new dsp_splitter(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);

            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                 fDSP2Inputs[chan] = fDSP1Outputs[chan % fDSP1->getNumOutputs()];
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

// Combine two 'compatible' DSP in merger

class dsp_merger : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

        void mix(int count, FAUSTFLOAT* dst, FAUSTFLOAT* src)
        {
            for (int frame = 0; frame < count; frame++) {
                dst[frame] += src[frame];
            }
        }

    public:

        dsp_merger(dsp* dsp1, dsp* dsp2, int buffer_size = 4096):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs(), buffer_size);
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), buffer_size);
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_merger()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Merge");
        }

        virtual dsp* clone()
        {
            return new dsp_merger(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, fDSP1Inputs, fDSP1Outputs);

            memset(fDSP2Inputs, 0, sizeof(FAUSTFLOAT*) * fDSP2->getNumInputs());

            for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                int mchan = chan % fDSP2->getNumInputs();
                if (fDSP2Inputs[mchan]) {
                    mix(count, fDSP2Inputs[mchan], fDSP1Outputs[chan]);
                } else {
                    fDSP2Inputs[mchan] = fDSP1Outputs[chan];
                }
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

// Combine two 'compatible' DSP in a recursive way

class dsp_recursiver : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_recursiver(dsp* dsp1, dsp* dsp2):dsp_binary_combiner(dsp1, dsp2)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs(), 1);
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs(), 1);
            fDSP2Inputs = allocateChannels(fDSP2->getNumInputs(), 1);
            fDSP2Outputs = allocateChannels(fDSP2->getNumOutputs(), 1);
        }

        virtual ~dsp_recursiver()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            deleteChannels(fDSP2Inputs, fDSP2->getNumInputs());
            deleteChannels(fDSP2Outputs, fDSP2->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() - fDSP2->getNumOutputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface, "Recursiver");
        }

        virtual dsp* clone()
        {
            return new dsp_recursiver(fDSP1->clone(), fDSP2->clone());
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int frame = 0; (frame < count); frame++) {

                for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan][0] = fDSP2Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP1->getNumInputs() - fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan + fDSP2->getNumOutputs()][0] = inputs[chan][frame];
                }

                fDSP1->compute(1, fDSP1Inputs, fDSP1Outputs);

                for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                    outputs[chan][frame] = fDSP1Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                    fDSP2Inputs[chan][0] = fDSP1Outputs[chan][0];
                }

                fDSP2->compute(1, fDSP2Inputs, fDSP2Outputs);
            }
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

#ifndef __dsp_algebra_api__
#define __dsp_algebra_api__
// DSP algebra API
/*
 Each operation takes two DSP as parameters, returns the combined DSPs, or null if failure with an error message.
 */

static dsp* createDSPSequencer(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() != dsp2->getNumInputs()) {
        std::stringstream error_aux;
        error_aux << "Connection error int dsp_sequencer : the number of outputs ("
                  << dsp1->getNumOutputs() << ") of A "
                  << "must be equal to the number of inputs (" << dsp2->getNumInputs() << ") of B" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_sequencer(dsp1, dsp2);
    }
}

static dsp* createDSPParallelizer(dsp* dsp1, dsp* dsp2, std::string& error)
{
    return new dsp_parallelizer(dsp1, dsp2);
}

static dsp* createDSPSplitter(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_splitter : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_splitter : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() % dsp1->getNumOutputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_splitter : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a divisor of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2);
    } else {
        return new dsp_splitter(dsp1, dsp2);
    }
}

static dsp* createDSPMerger(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_merger : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_merger : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp1->getNumOutputs() % dsp2->getNumInputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_merger : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a multiple of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2);
    } else {
        return new dsp_merger(dsp1, dsp2);
    }
}

static dsp* createDSPRecursiver(dsp* dsp1, dsp* dsp2, std::string& error)
{
    if ((dsp2->getNumInputs() > dsp1->getNumOutputs()) || (dsp2->getNumOutputs() > dsp1->getNumInputs())) {
        std::stringstream error_aux;
        error_aux << "Connection error in : dsp_recursiver" << std::endl;
        if (dsp2->getNumInputs() > dsp1->getNumOutputs()) {
            error_aux << "The number of outputs " << dsp1->getNumOutputs()
                      << " of the first expression should be greater or equal to the number of inputs ("
                      << dsp2->getNumInputs()
                      << ") of the second expression" << std::endl;
        }
        if (dsp2->getNumOutputs() > dsp1->getNumInputs()) {
            error_aux << "The number of inputs " << dsp1->getNumInputs()
                      << " of the first expression should be greater or equal to the number of outputs ("
                      << dsp2->getNumOutputs()
                      << ") of the second expression" << std::endl;
        }
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_recursiver(dsp1, dsp2);
    }
}
#endif

#endif
/**************************  END  dsp-combiner.h **************************/
/************************** BEGIN dsp-adapter.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_adapter__
#define __dsp_adapter__

#ifndef _WIN32
#include <alloca.h>
#endif
#include <string.h>
#include <iostream>
#include <cmath>


// Adapts a DSP for a different number of inputs/outputs
class dsp_adapter : public decorator_dsp {
    
    private:
        
        FAUSTFLOAT** fAdaptedInputs;
        FAUSTFLOAT** fAdaptedOutputs;
        int fHardwareInputs;
        int fHardwareOutputs;
        
        void adaptBuffers(FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int i = 0; i < fHardwareInputs; i++) {
                fAdaptedInputs[i] = inputs[i];
            }
            for (int i = 0; i < fHardwareOutputs; i++) {
                fAdaptedOutputs[i] = outputs[i];
            }
        }
        
    public:
        
        dsp_adapter(dsp* dsp, int hardware_inputs, int hardware_outputs, int buffer_size):decorator_dsp(dsp)
        {
            fHardwareInputs = hardware_inputs;
            fHardwareOutputs = hardware_outputs;
             
            fAdaptedInputs = new FAUSTFLOAT*[dsp->getNumInputs()];
            for (int i = 0; i < dsp->getNumInputs() - fHardwareInputs; i++) {
                fAdaptedInputs[i + fHardwareInputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedInputs[i + fHardwareInputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
            
            fAdaptedOutputs = new FAUSTFLOAT*[dsp->getNumOutputs()];
            for (int i = 0; i < dsp->getNumOutputs() - fHardwareOutputs; i++) {
                fAdaptedOutputs[i + fHardwareOutputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedOutputs[i + fHardwareOutputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
        }
        
        virtual ~dsp_adapter()
        {
            for (int i = 0; i < fDSP->getNumInputs() - fHardwareInputs; i++) {
                delete [] fAdaptedInputs[i + fHardwareInputs];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs() - fHardwareOutputs; i++) {
                delete [] fAdaptedOutputs[i + fHardwareOutputs];
            }
            delete [] fAdaptedOutputs;
        }
    
        virtual int getNumInputs() { return fHardwareInputs; }
        virtual int getNumOutputs() { return fHardwareOutputs; }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(date_usec, count, fAdaptedInputs, fAdaptedOutputs);
        }
        
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(count, fAdaptedInputs, fAdaptedOutputs);
        }
};

// Adapts a DSP for a different sample size
template <typename REAL_INT, typename REAL_EXT>
class dsp_sample_adapter : public decorator_dsp {
    
    protected:
    
        REAL_INT** fAdaptedInputs;
        REAL_INT** fAdaptedOutputs;
    
        void adaptInputBuffers(int count, FAUSTFLOAT** inputs)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    fAdaptedInputs[chan][frame] = REAL_INT(reinterpret_cast<REAL_EXT**>(inputs)[chan][frame]);
                }
            }
        }
    
        void adaptOutputsBuffers(int count, FAUSTFLOAT** outputs)
        {
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    reinterpret_cast<REAL_EXT**>(outputs)[chan][frame] = REAL_EXT(fAdaptedOutputs[chan][frame]);
                }
            }
        }
    
    public:
    
        dsp_sample_adapter(dsp* dsp):decorator_dsp(dsp)
        {
            fAdaptedInputs = new REAL_INT*[dsp->getNumInputs()];
            for (int i = 0; i < dsp->getNumInputs(); i++) {
                fAdaptedInputs[i] = new REAL_INT[4096];
            }
            
            fAdaptedOutputs = new REAL_INT*[dsp->getNumOutputs()];
            for (int i = 0; i < dsp->getNumOutputs(); i++) {
                fAdaptedOutputs[i] = new REAL_INT[4096];
            }
        }
    
        virtual ~dsp_sample_adapter()
        {
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                delete [] fAdaptedInputs[i];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                delete [] fAdaptedOutputs[i];
            }
            delete [] fAdaptedOutputs;
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses REAL_INT
            fDSP->compute(count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
        }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses REAL_INT
            fDSP->compute(date_usec, count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
       }
};

// Template used to specialize double parameters expressed as NUM/DENOM
template <int NUM, int DENOM>
struct Double {
    static constexpr double value() { return double(NUM)/double(DENOM); }
};

// Base class for filters
template <class fVslider0, int fVslider1>
struct Filter {
    inline int getFactor() { return fVslider1; }
};

// Identity filter: copy input to output
template <class fVslider0, int fVslider1>
struct Identity : public Filter<fVslider0, fVslider1> {
    inline int getFactor() { return fVslider1; }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        memcpy(output0, input0, count * sizeof(FAUSTFLOAT));
    }
};

// Generated with process = fi.lowpass(3, ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3 : public Filter<fVslider0, fVslider1> {

    REAL fVec0[2];
    REAL fRec1[2];
    REAL fRec0[3];

    inline REAL LowPass3_faustpower2_f(REAL value)
    {
        return (value * value);
    }

    LowPass3()
    {
        for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
            fVec0[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }

    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 1.0000000000000002) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (fSlow1 + 1.0));
        REAL fSlow4 = (1.0 - fSlow1);
        REAL fSlow5 = (((fSlow1 + -1.0000000000000002) / fSlow0) + 1.0);
        REAL fSlow6 = (2.0 * (1.0 - (1.0 / LowPass3_faustpower2_f(fSlow0))));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            REAL fTemp0 = REAL(input0[i]);
            fVec0[0] = fTemp0;
            fRec1[0] = (0.0 - (fSlow3 * ((fSlow4 * fRec1[1]) - (fTemp0 + fVec0[1]))));
            fRec0[0] = (fRec1[0] - (fSlow2 * ((fSlow5 * fRec0[2]) + (fSlow6 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fVec0[1] = fVec0[0];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass(4, ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass4 : public Filter<fVslider0, fVslider1> {
    
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL LowPass4_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass4()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0f;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec0[l1] = 0.0f;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.76536686473017945) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (((fSlow1 + 1.8477590650225735) / fSlow0) + 1.0));
        REAL fSlow4 = (((fSlow1 + -1.8477590650225735) / fSlow0) + 1.0);
        REAL fSlow5 = (2.0 * (1.0 - (1.0 / LowPass4_faustpower2_f(fSlow0))));
        REAL fSlow6 = (((fSlow1 + -0.76536686473017945) / fSlow0) + 1.0);
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow3 * ((fSlow4 * fRec1[2]) + (fSlow5 * fRec1[1]))));
            fRec0[0] = ((fSlow3 * (fRec1[2] + (fRec1[0] + (2.0 * fRec1[1])))) - (fSlow2 * ((fSlow6 * fRec0[2]) + (fSlow5 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass3e(ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3e : public Filter<fVslider0, fVslider1> {

    REAL fRec1[3];
    REAL fVec0[2];
    REAL fRec0[2];
    
    inline REAL LowPass3e_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass3e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fVec0[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (fSlow1 + 0.82244590899881598));
        REAL fSlow3 = (0.82244590899881598 - fSlow1);
        REAL fSlow4 = (1.0 / (((fSlow1 + 0.80263676416103003) / fSlow0) + 1.4122708937742039));
        REAL fSlow5 = LowPass3e_faustpower2_f(fSlow0);
        REAL fSlow6 = (0.019809144837788999 / fSlow5);
        REAL fSlow7 = (fSlow6 + 1.1615164189826961);
        REAL fSlow8 = (((fSlow1 + -0.80263676416103003) / fSlow0) + 1.4122708937742039);
        REAL fSlow9 = (2.0 * (1.4122708937742039 - (1.0 / fSlow5)));
        REAL fSlow10 = (2.0 * (1.1615164189826961 - fSlow6));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow4 * ((fSlow8 * fRec1[2]) + (fSlow9 * fRec1[1]))));
            REAL fTemp0 = (fSlow4 * (((fSlow7 * fRec1[0]) + (fSlow10 * fRec1[1])) + (fSlow7 * fRec1[2])));
            fVec0[0] = fTemp0;
            fRec0[0] = (0.0 - (fSlow2 * ((fSlow3 * fRec0[1]) - (fTemp0 + fVec0[1]))));
            output0[i] = FAUSTFLOAT(fRec0[0]);
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fVec0[1] = fVec0[0];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass6e(ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass6e : public Filter<fVslider0, fVslider1> {

    REAL fRec2[3];
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL LowPass6e_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass6e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec2[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.16840487111358901) / fSlow0) + 1.0693584077073119));
        REAL fSlow3 = LowPass6e_faustpower2_f(fSlow0);
        REAL fSlow4 = (1.0 / fSlow3);
        REAL fSlow5 = (fSlow4 + 53.536152954556727);
        REAL fSlow6 = (1.0 / (((fSlow1 + 0.51247864188914105) / fSlow0) + 0.68962136448467504));
        REAL fSlow7 = (fSlow4 + 7.6217312988706034);
        REAL fSlow8 = (1.0 / (((fSlow1 + 0.78241304682164503) / fSlow0) + 0.24529150870616001));
        REAL fSlow9 = (9.9999997054999994e-05 / fSlow3);
        REAL fSlow10 = (fSlow9 + 0.00043322720055500002);
        REAL fSlow11 = (((fSlow1 + -0.78241304682164503) / fSlow0) + 0.24529150870616001);
        REAL fSlow12 = (2.0 * (0.24529150870616001 - fSlow4));
        REAL fSlow13 = (2.0 * (0.00043322720055500002 - fSlow9));
        REAL fSlow14 = (((fSlow1 + -0.51247864188914105) / fSlow0) + 0.68962136448467504);
        REAL fSlow15 = (2.0 * (0.68962136448467504 - fSlow4));
        REAL fSlow16 = (2.0 * (7.6217312988706034 - fSlow4));
        REAL fSlow17 = (((fSlow1 + -0.16840487111358901) / fSlow0) + 1.0693584077073119);
        REAL fSlow18 = (2.0 * (1.0693584077073119 - fSlow4));
        REAL fSlow19 = (2.0 * (53.536152954556727 - fSlow4));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec2[0] = (REAL(input0[i]) - (fSlow8 * ((fSlow11 * fRec2[2]) + (fSlow12 * fRec2[1]))));
            fRec1[0] = ((fSlow8 * (((fSlow10 * fRec2[0]) + (fSlow13 * fRec2[1])) + (fSlow10 * fRec2[2]))) - (fSlow6 * ((fSlow14 * fRec1[2]) + (fSlow15 * fRec1[1]))));
            fRec0[0] = ((fSlow6 * (((fSlow7 * fRec1[0]) + (fSlow16 * fRec1[1])) + (fSlow7 * fRec1[2]))) - (fSlow2 * ((fSlow17 * fRec0[2]) + (fSlow18 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (((fSlow5 * fRec0[0]) + (fSlow19 * fRec0[1])) + (fSlow5 * fRec0[2]))));
            fRec2[2] = fRec2[1];
            fRec2[1] = fRec2[0];
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// A "si.bus(N)" like hard-coded class
struct dsp_bus : public dsp {
    
    int fChannels;
    int fSampleRate;
    
    dsp_bus(int channels):fChannels(channels), fSampleRate(-1)
    {}
    
    virtual int getNumInputs() { return fChannels; }
    virtual int getNumOutputs() { return fChannels; }
    
    virtual int getSampleRate() { return fSampleRate; }
    
    virtual void buildUserInterface(UI* ui_interface) {}
    virtual void init(int sample_rate)
    {
        //classInit(sample_rate);
        instanceInit(sample_rate);
    }
    
    virtual void instanceInit(int sample_rate)
    {
        fSampleRate = sample_rate;
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }
    
    virtual void instanceConstants(int sample_rate) {}
    virtual void instanceResetUserInterface() {}
    virtual void instanceClear() {}
    
    virtual dsp* clone() { return new dsp_bus(fChannels); }
    
    virtual void metadata(Meta* m) {}
    
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        for (int chan = 0; chan < fChannels; chan++) {
            memcpy(outputs[chan], inputs[chan], sizeof(FAUSTFLOAT) * count);
        }
    }
    
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        compute(count, inputs, outputs);
    }
    
};

// Base class for sample-rate adapter
template <typename FILTER>
class sr_sampler : public decorator_dsp {

    protected:
    
        std::vector<FILTER> fInputLowPass;
        std::vector<FILTER> fOutputLowPass;
    
        inline int getFactor() { return this->fOutputLowPass[0].getFactor(); }
    
    public:
    
        sr_sampler(dsp* dsp):decorator_dsp(dsp)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                fInputLowPass.push_back(FILTER());
            }
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                fOutputLowPass.push_back(FILTER());
            }
        }
 };

// Down sample-rate adapter
template <typename FILTER>
class dsp_down_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_down_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate / this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate / this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate / this->getFactor());
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count / this->getFactor();
           
            // Adapt inputs
            FAUSTFLOAT* fInputs[this->fDSP->getNumInputs()];
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Lowpass filtering in place on 'inputs'
                this->fInputLowPass[chan].compute(count, inputs[chan], inputs[chan]);
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Decimate
                for (int frame = 0; frame < real_count; frame++) {
                    fInputs[chan][frame] = inputs[chan][frame * this->getFactor()];
                }
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT* fOutputs[this->fDSP->getNumOutputs()];
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at lower rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Puts zeros
                memset(outputs[chan], 0, sizeof(FAUSTFLOAT) * count);
                for (int frame = 0; frame < real_count; frame++) {
                    // Copy one sample every 'DownFactor'
                    // Apply volume
                    //outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame] * this->getFactor();
                    outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame];
                }
                // Lowpass filtering in place on 'outputs'
                this->fOutputLowPass[chan].compute(count, outputs[chan], outputs[chan]);
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

// Up sample-rate adapter
template <typename FILTER>
class dsp_up_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_up_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate * this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate * this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate * this->getFactor());
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count * this->getFactor();
            
            // Adapt inputs
            FAUSTFLOAT** fInputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumInputs() * sizeof(FAUSTFLOAT*));
            
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Puts zeros
                memset(fInputs[chan], 0, sizeof(FAUSTFLOAT) * real_count);
                for (int frame = 0; frame < count; frame++) {
                    // Copy one sample every 'UpFactor'
                    fInputs[chan][frame * this->getFactor()] = inputs[chan][frame];
                }
                // Lowpass filtering in place on 'fInputs'
                this->fInputLowPass[chan].compute(real_count, fInputs[chan], fInputs[chan]);
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT** fOutputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumOutputs() * sizeof(FAUSTFLOAT*));
            
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at upper rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Lowpass filtering in place on 'fOutputs'
                this->fOutputLowPass[chan].compute(real_count, fOutputs[chan], fOutputs[chan]);
                // Decimate
                for (int frame = 0; frame < count; frame++) {
                    // Apply volume
                    //outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()] * this->getFactor();
                    outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()];
                }
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

#endif
/**************************  END  dsp-adapter.h **************************/
/************************** BEGIN misc.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <cstdlib>
#include <string.h>
#include <fstream>
#include <string>

/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) { int r = 0; while ((1<<r) < x) r++; return r; }

static long lopt(char* argv[], const char* name, long def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::atoi(argv[i+1]);
    return def;
}

static long lopt1(int argc, char* argv[], const char* longname, const char* shortname, long def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return atoi(argv[i]);
        }
    }
    return def;
}

static const char* lopts(char* argv[], const char* name, const char* def)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
    return def;
}

static const char* lopts1(int argc, char* argv[], const char* longname, const char* shortname, const char* def)
{
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i-1], shortname) == 0 || strcmp(argv[i-1], longname) == 0) {
            return argv[i];
        }
    }
    return def;
}

static bool isopt(char* argv[], const char* name)
{
    for (int i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
    return false;
}

static std::string pathToContent(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ifstream::binary);
    
    file.seekg(0, file.end);
    int size = int(file.tellg());
    file.seekg(0, file.beg);
    
    // And allocate buffer to that a single line can be read...
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    
    // Terminate the string
    buffer[size] = 0;
    std::string result = buffer;
    file.close();
    delete [] buffer;
    return result;
}

#endif

/**************************  END  misc.h **************************/
/************************** BEGIN SaveUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2019-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_SAVEUI_H
#define FAUST_SAVEUI_H

/************************** BEGIN DecoratorUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef Decorator_UI_H
#define Decorator_UI_H


//----------------------------------------------------------------
//  Generic UI empty implementation
//----------------------------------------------------------------

class GenericUI : public UI
{
    
    public:
        
        GenericUI() {}
        virtual ~GenericUI() {}
        
        // -- widget's layouts
        virtual void openTabBox(const char* label) {}
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* soundpath, Soundfile** sf_zone) {}
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
    
};

//----------------------------------------------------------------
//  Generic UI decorator
//----------------------------------------------------------------

class DecoratorUI : public UI
{
    
    protected:
        
        UI* fUI;
        
    public:
        
        DecoratorUI(UI* ui = 0):fUI(ui) {}
        virtual ~DecoratorUI() { delete fUI; }
        
        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addNumEntry(label, zone, init, min, max, step); }
        
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addVerticalBargraph(label, zone, min, max); }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) { fUI->addSoundfile(label, filename, sf_zone); }
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }
    
};

#endif
/**************************  END  DecoratorUI.h **************************/

// Base class to handle controllers state save/load

class SaveUI : public GenericUI {

    protected:
    
        struct SavedZone {
            FAUSTFLOAT* fZone;
            FAUSTFLOAT fCurrent;
            FAUSTFLOAT fInit;
            
            SavedZone():fZone(nullptr), fCurrent(FAUSTFLOAT(0)), fInit(FAUSTFLOAT(0))
            {}
            SavedZone(FAUSTFLOAT* zone, FAUSTFLOAT current, FAUSTFLOAT init)
            :fZone(zone), fCurrent(current), fInit(init)
            {
                *fZone = current;
            }
            ~SavedZone()
            {}
        };
        
        std::map<std::string, SavedZone> fName2Zone;
    
        virtual void addItem(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init) = 0;
    
    public:
    
        SaveUI() {}
        virtual ~SaveUI() {}
    
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addItem(label, zone, FAUSTFLOAT(0));
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addItem(label, zone, FAUSTFLOAT(0));
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addItem(label, zone, init);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addItem(label, zone, init);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addItem(label, zone, init);
        }

        void reset()
        {
            for (auto& it : fName2Zone) {
                *it.second.fZone = it.second.fInit;
            }
        }
        
        void display()
        {
            for (auto& it : fName2Zone) {
                std::cout << "SaveUI::display path = " << it.first << " value = " << *it.second.fZone << std::endl;
            }
        }
        
        void save()
        {
            for (auto& it : fName2Zone) {
                it.second.fCurrent = *it.second.fZone;
            }
        }
};

/*
 Save/load current value using the label, reset to init value
 */

class SaveLabelUI : public SaveUI {
    
    protected:
    
        void addItem(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init)
        {
            if (fName2Zone.find(label) != fName2Zone.end()) {
                FAUSTFLOAT current = fName2Zone[label].fCurrent;
                fName2Zone[label] = SavedZone(zone, current, init);
            } else {
                fName2Zone[label] = SavedZone(zone, init, init);
            }
        }
        
    public:
        
        SaveLabelUI() : SaveUI() {}
        virtual ~SaveLabelUI() {}        
   
};

/*
 Save/load current value using the complete path, reset to init value
*/

class SavePathUI : public SaveUI, public PathBuilder {
    
    protected:
    
        void openTabBox(const char* label) { pushLabel(label); }
        void openHorizontalBox(const char* label) { pushLabel(label);; }
        void openVerticalBox(const char* label) { pushLabel(label); }
        void closeBox() { popLabel(); };
    
        void addItem(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init)
        {
            std::string path = buildPath(label);
            if (fName2Zone.find(path) != fName2Zone.end()) {
                FAUSTFLOAT current = fName2Zone[path].fCurrent;
                fName2Zone[path] = SavedZone(zone, current, init);
            } else {
                fName2Zone[path] = SavedZone(zone, init, init);
            }
        }
   
    public:

        SavePathUI(): SaveUI() {}
        virtual ~SavePathUI() {}

};

#endif

/**************************  END  SaveUI.h **************************/

// Always included
/************************** BEGIN OSCUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __OSCUI__
#define __OSCUI__

#include <vector>
#include <string>

/*

  Faust Project

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __OSCControler__
#define __OSCControler__

#include <string>
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __FaustFactory__
#define __FaustFactory__

#include <stack>
#include <string>
#include <sstream>

/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __FaustNode__
#define __FaustNode__

#include <string>
#include <vector>

/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __MessageDriven__
#define __MessageDriven__

#include <string>
#include <vector>

/*

  Copyright (C) 2010  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __MessageProcessor__
#define __MessageProcessor__

namespace oscfaust
{

class Message;
//--------------------------------------------------------------------------
/*!
	\brief an abstract class for objects able to process OSC messages	
*/
class MessageProcessor
{
	public:
		virtual		~MessageProcessor() {}
		virtual void processMessage( const Message* msg ) = 0;
};

} // end namespoace

#endif
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __smartpointer__
#define __smartpointer__

#include <cassert>

namespace oscfaust
{

/*!
\brief the base class for smart pointers implementation

	Any object that want to support smart pointers should
	inherit from the smartable class which provides reference counting
	and automatic delete when the reference count drops to zero.
*/
class smartable {
	private:
		unsigned 	refCount;		
	public:
		//! gives the reference count of the object
		unsigned refs() const         { return refCount; }
		//! addReference increments the ref count and checks for refCount overflow
		void addReference()           { refCount++; assert(refCount != 0); }
		//! removeReference delete the object when refCount is zero		
		void removeReference()		  { if (--refCount == 0) delete this; }
		
	protected:
		smartable() : refCount(0) {}
		smartable(const smartable&): refCount(0) {}
		//! destructor checks for non-zero refCount
		virtual ~smartable()    
        { 
            /* 
                See "Static SFaustNode create (const char* name, C* zone, C init, C min, C max, const char* prefix, GUI* ui)" comment.
                assert (refCount == 0); 
            */
        }
		smartable& operator=(const smartable&) { return *this; }
};

/*!
\brief the smart pointer implementation

	A smart pointer is in charge of maintaining the objects reference count 
	by the way of pointers operators overloading. It supports class 
	inheritance and conversion whenever possible.
\n	Instances of the SMARTP class are supposed to use \e smartable types (or at least
	objects that implements the \e addReference and \e removeReference
	methods in a consistent way).
*/
template<class T> class SMARTP {
	private:
		//! the actual pointer to the class
		T* fSmartPtr;

	public:
		//! an empty constructor - points to null
		SMARTP()	: fSmartPtr(0) {}
		//! build a smart pointer from a class pointer
		SMARTP(T* rawptr) : fSmartPtr(rawptr)              { if (fSmartPtr) fSmartPtr->addReference(); }
		//! build a smart pointer from an convertible class reference
		template<class T2> 
		SMARTP(const SMARTP<T2>& ptr) : fSmartPtr((T*)ptr) { if (fSmartPtr) fSmartPtr->addReference(); }
		//! build a smart pointer from another smart pointer reference
		SMARTP(const SMARTP& ptr) : fSmartPtr((T*)ptr)     { if (fSmartPtr) fSmartPtr->addReference(); }

		//! the smart pointer destructor: simply removes one reference count
		~SMARTP()  { if (fSmartPtr) fSmartPtr->removeReference(); }
		
		//! cast operator to retrieve the actual class pointer
		operator T*() const  { return fSmartPtr;	}

		//! '*' operator to access the actual class pointer
		T& operator*() const {
			// checks for null dereference
			assert (fSmartPtr != 0);
			return *fSmartPtr;
		}

		//! operator -> overloading to access the actual class pointer
		T* operator->() const	{ 
			// checks for null dereference
			assert (fSmartPtr != 0);
			return fSmartPtr;
		}

		//! operator = that moves the actual class pointer
		template <class T2>
		SMARTP& operator=(T2 p1_)	{ *this=(T*)p1_; return *this; }

		//! operator = that moves the actual class pointer
		SMARTP& operator=(T* p_)	{
			// check first that pointers differ
			if (fSmartPtr != p_) {
				// increments the ref count of the new pointer if not null
				if (p_ != 0) p_->addReference();
				// decrements the ref count of the old pointer if not null
				if (fSmartPtr != 0) fSmartPtr->removeReference();
				// and finally stores the new actual pointer
				fSmartPtr = p_;
			}
			return *this;
		}
		//! operator < to support SMARTP map with Visual C++
		bool operator<(const SMARTP<T>& p_)	const			  { return fSmartPtr < ((T *) p_); }
		//! operator = to support inherited class reference
		SMARTP& operator=(const SMARTP<T>& p_)                { return operator=((T *) p_); }
		//! dynamic cast support
		template<class T2> SMARTP& cast(T2* p_)               { return operator=(dynamic_cast<T*>(p_)); }
		//! dynamic cast support
		template<class T2> SMARTP& cast(const SMARTP<T2>& p_) { return operator=(dynamic_cast<T*>(p_)); }
};

}

#endif

namespace oscfaust
{

class Message;
class OSCRegexp;
class MessageDriven;
typedef class SMARTP<MessageDriven>	SMessageDriven;

//--------------------------------------------------------------------------
/*!
	\brief a base class for objects accepting OSC messages
	
	Message driven objects are hierarchically organized in a tree.
	They provides the necessary to dispatch an OSC message to its destination
	node, according to the message OSC address. 
	
	The principle of the dispatch is the following:
	- first the processMessage() method should be called on the top level node
	- next processMessage call propose 
*/
class MessageDriven : public MessageProcessor, public smartable
{
	std::string						fName;			///< the node name
	std::string						fOSCPrefix;		///< the node OSC address prefix (OSCAddress = fOSCPrefix + '/' + fName)
	std::vector<SMessageDriven>		fSubNodes;		///< the subnodes of the current node

	protected:
				 MessageDriven(const char *name, const char *oscprefix) : fName (name), fOSCPrefix(oscprefix) {}
		virtual ~MessageDriven() {}

	public:
		static SMessageDriven create(const char* name, const char *oscprefix)	{ return new MessageDriven(name, oscprefix); }

		/*!
			\brief OSC message processing method.
			\param msg the osc message to be processed
			The method should be called on the top level node.
		*/
		virtual void	processMessage(const Message* msg);

		/*!
			\brief propose an OSc message at a given hierarchy level.
			\param msg the osc message currently processed
			\param regexp a regular expression based on the osc address head
			\param addrTail the osc address tail
			
			The method first tries to match the regular expression with the object name. 
			When it matches:
			- it calls \c accept when \c addrTail is empty 
			- or it \c propose the message to its subnodes when \c addrTail is not empty. 
			  In this case a new \c regexp is computed with the head of \c addrTail and a new \c addrTail as well.
		*/
		virtual void	propose(const Message* msg, const OSCRegexp* regexp, const std::string& addrTail);

		/*!
			\brief accept an OSC message. 
			\param msg the osc message currently processed
			\return true when the message is processed by the node
			
			The method is called only for the destination nodes. The real message acceptance is the node 
			responsability and may depend on the message content.
		*/
		virtual bool	accept(const Message* msg);

		/*!
			\brief handler for the \c 'get' message
			\param ipdest the output message destination IP
			
			The \c 'get' message is supported by every node:
			- it is propagated to the subnodes until it reaches terminal nodes
			- a terminal node send its state on \c 'get' request to the IP address given as parameter.
			The \c get method is basically called by the accept method.
		*/
		virtual void	get(unsigned long ipdest) const;

		/*!
			\brief handler for the \c 'get' 'attribute' message
			\param ipdest the output message destination IP
			\param what the requested attribute
			
			The \c 'get' message is supported by every node:
			- it is propagated to the subnodes until it reaches terminal nodes
			- a terminal node send its state on \c 'get' request to the IP address given as parameter.
			The \c get method is basically called by the accept method.
		*/
		virtual void	get(unsigned long ipdest, const std::string& what) const {}

		void			add(SMessageDriven node)	{ fSubNodes.push_back (node); }
		const char*		getName() const				{ return fName.c_str(); }
		std::string		getOSCAddress() const;
		int				size() const				{ return (int)fSubNodes.size (); }
		
		const std::string&	name() const			{ return fName; }
		SMessageDriven	subnode(int i)              { return fSubNodes[i]; }
};

} // end namespoace

#endif
/*

  Copyright (C) 2011  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#ifndef __Message__
#define __Message__

#include <string>
#include <vector>

namespace oscfaust
{

class OSCStream;
template <typename T> class MsgParam;
class baseparam;
typedef SMARTP<baseparam>	Sbaseparam;

//--------------------------------------------------------------------------
/*!
	\brief base class of a message parameters
*/
class baseparam : public smartable
{
	public:
		virtual ~baseparam() {}

		/*!
		 \brief utility for parameter type checking
		*/
		template<typename X> bool isType() const { return dynamic_cast<const MsgParam<X>*> (this) != 0; }
		/*!
		 \brief utility for parameter convertion
		 \param errvalue the returned value when no conversion applies
		 \return the parameter value when the type matches
		*/
		template<typename X> X	value(X errvalue) const 
			{ const MsgParam<X>* o = dynamic_cast<const MsgParam<X>*> (this); return o ? o->getValue() : errvalue; }
		/*!
		 \brief utility for parameter comparison
		*/
		template<typename X> bool	equal(const baseparam& p) const 
			{ 
				const MsgParam<X>* a = dynamic_cast<const MsgParam<X>*> (this); 
				const MsgParam<X>* b = dynamic_cast<const MsgParam<X>*> (&p);
				return a && b && (a->getValue() == b->getValue());
			}
		/*!
		 \brief utility for parameter comparison
		*/
		bool operator==(const baseparam& p) const 
			{ 
				return equal<float>(p) || equal<int>(p) || equal<std::string>(p);
			}
		bool operator!=(const baseparam& p) const
			{ 
				return !equal<float>(p) && !equal<int>(p) && !equal<std::string>(p);
			}
			
		virtual SMARTP<baseparam> copy() const = 0;
};

//--------------------------------------------------------------------------
/*!
	\brief template for a message parameter
*/
template <typename T> class MsgParam : public baseparam
{
	T fParam;
	public:
				 MsgParam(T val) : fParam(val)	{}
		virtual ~MsgParam() {}
		
		T getValue() const { return fParam; }
		
		virtual Sbaseparam copy() const { return new MsgParam<T>(fParam); }
};

//--------------------------------------------------------------------------
/*!
	\brief a message description
	
	A message is composed of an address (actually an OSC address),
	a message string that may be viewed as a method name
	and a list of message parameters.
*/
class Message
{
    public:
        typedef SMARTP<baseparam>		argPtr;		///< a message argument ptr type
        typedef std::vector<argPtr>		argslist;	///< args list type

    private:
        unsigned long	fSrcIP;			///< the message source IP number
        std::string	fAddress;			///< the message osc destination address
        std::string	fAlias;             ///< the message alias osc destination address
        argslist	fArguments;			///< the message arguments

    public:
            /*!
                \brief an empty message constructor
            */
             Message() {}
            /*!
                \brief a message constructor
                \param address the message destination address
            */
            Message(const std::string& address) : fAddress(address), fAlias("") {}
             
            Message(const std::string& address, const std::string& alias) : fAddress(address), fAlias(alias) {}
            /*!
                \brief a message constructor
                \param address the message destination address
                \param args the message parameters
            */
            Message(const std::string& address, const argslist& args) 
                : fAddress(address), fArguments(args) {}
            /*!
                \brief a message constructor
                \param msg a message
            */
             Message(const Message& msg);
    virtual ~Message() {} //{ freed++; std::cout << "running messages: " << (allocated - freed) << std::endl; }

    /*!
        \brief adds a parameter to the message
        \param val the parameter
    */
    template <typename T> void add(T val)	{ fArguments.push_back(new MsgParam<T>(val)); }
    /*!
        \brief adds a float parameter to the message
        \param val the parameter value
    */
    void	add(float val)					{ add<float>(val); }
    
    /*!
     \brief adds a double parameter to the message
     \param val the parameter value
     */
    void	add(double val)					{ add<double>(val); }
    
    /*!
        \brief adds an int parameter to the message
        \param val the parameter value
    */
    void	add(int val)					{ add<int>(val); }
    
    /*!
        \brief adds a string parameter to the message
        \param val the parameter value
    */
    void	add(const std::string& val)		{ add<std::string>(val); }

    /*!
        \brief adds a parameter to the message
        \param val the parameter
    */
    void	add(argPtr val)                 { fArguments.push_back( val ); }

    /*!
        \brief sets the message address
        \param addr the address
    */
    void				setSrcIP(unsigned long addr)		{ fSrcIP = addr; }

    /*!
        \brief sets the message address
        \param addr the address
    */
    void				setAddress(const std::string& addr)		{ fAddress = addr; }
    /*!
        \brief print the message
        \param out the output stream
    */
    void				print(std::ostream& out) const;
    /*!
        \brief send the message to OSC
        \param out the OSC output stream
    */
    void				print(OSCStream& out) const;
    /*!
        \brief print message arguments
        \param out the OSC output stream
    */
    void				printArgs(OSCStream& out) const;

    /// \brief gives the message address
    const std::string&	address() const		{ return fAddress; }
    /// \brief gives the message alias
    const std::string&	alias() const		{ return fAlias; }
    /// \brief gives the message parameters list
    const argslist&		params() const		{ return fArguments; }
    /// \brief gives the message parameters list
    argslist&			params()			{ return fArguments; }
    /// \brief gives the message source IP 
    unsigned long		src() const			{ return fSrcIP; }
    /// \brief gives the message parameters count
    int					size() const		{ return (int)fArguments.size(); }

    bool operator == (const Message& other) const;	

    /*!
        \brief gives a message float parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, float& val) const		{ val = params()[i]->value<float>(val); return params()[i]->isType<float>(); }
    
    /*!
     \brief gives a message double parameter
     \param i the parameter index (0 <= i < size())
     \param val on output: the parameter value when the parameter type matches
     \return false when types don't match
     */
    bool	param(int i, double& val) const		{ val = params()[i]->value<double>(val); return params()[i]->isType<double>(); }
    
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, int& val) const		{ val = params()[i]->value<int>(val); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, unsigned int& val) const		{ val = params()[i]->value<int>(val); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
        \note a boolean value is handled as integer
    */
    bool	param(int i, bool& val) const		{ int ival = 0; ival = params()[i]->value<int>(ival); val = ival!=0; return params()[i]->isType<int>(); }
    /*!
        \brief gives a message int parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, long int& val) const	{ val = long(params()[i]->value<int>(val)); return params()[i]->isType<int>(); }
    /*!
        \brief gives a message string parameter
        \param i the parameter index (0 <= i < size())
        \param val on output: the parameter value when the parameter type matches
        \return false when types don't match
    */
    bool	param(int i, std::string& val) const { val = params()[i]->value<std::string>(val); return params()[i]->isType<std::string>(); }
};


} // end namespoace

#endif
/************************** BEGIN GUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __GUI_H__
#define __GUI_H__

#include <list>
#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

#ifdef _WIN32
# pragma warning (disable: 4100)
#else
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/************************** BEGIN ValueConverter.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __ValueConverter__
#define __ValueConverter__

/***************************************************************************************
								ValueConverter.h
                            (GRAME, Copyright 2015-2019)

Set of conversion objects used to map user interface values (for example a gui slider
delivering values between 0 and 1) to faust values (for example a vslider between
20 and 20000) using a log scale.

-- Utilities

Range(lo,hi) : clip a value x between lo and hi
Interpolator(lo,hi,v1,v2) : Maps a value x between lo and hi to a value y between v1 and v2
Interpolator3pt(lo,mi,hi,v1,vm,v2) : Map values between lo mid hi to values between v1 vm v2

-- Value Converters

ValueConverter::ui2faust(x)
ValueConverter::faust2ui(x)

-- ValueConverters used for sliders depending of the scale

LinearValueConverter(umin, umax, fmin, fmax)
LinearValueConverter2(lo, mi, hi, v1, vm, v2) using 2 segments
LogValueConverter(umin, umax, fmin, fmax)
ExpValueConverter(umin, umax, fmin, fmax)

-- ValueConverters used for accelerometers based on 3 points

AccUpConverter(amin, amid, amax, fmin, fmid, fmax)		-- curve 0
AccDownConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 1
AccUpDownConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 2
AccDownUpConverter(amin, amid, amax, fmin, fmid, fmax)	-- curve 3

-- lists of ZoneControl are used to implement accelerometers metadata for each axes

ZoneControl(zone, valueConverter) : a zone with an accelerometer data converter

-- ZoneReader are used to implement screencolor metadata

ZoneReader(zone, valueConverter) : a zone with a data converter

****************************************************************************************/

#include <float.h>
#include <algorithm>    // std::max
#include <cmath>
#include <vector>
#include <assert.h>

//--------------------------------------------------------------------------------------
// Interpolator(lo,hi,v1,v2)
// Maps a value x between lo and hi to a value y between v1 and v2
// y = v1 + (x-lo)/(hi-lo)*(v2-v1)
// y = v1 + (x-lo) * coef   		with coef = (v2-v1)/(hi-lo)
// y = v1 + x*coef - lo*coef
// y = v1 - lo*coef + x*coef
// y = offset + x*coef				with offset = v1 - lo*coef
//--------------------------------------------------------------------------------------
class Interpolator
{
    private:

        //--------------------------------------------------------------------------------------
        // Range(lo,hi) clip a value between lo and hi
        //--------------------------------------------------------------------------------------
        struct Range
        {
            double fLo;
            double fHi;

            Range(double x, double y) : fLo(std::min<double>(x,y)), fHi(std::max<double>(x,y)) {}
            double operator()(double x) { return (x<fLo) ? fLo : (x>fHi) ? fHi : x; }
        };


        Range fRange;
        double fCoef;
        double fOffset;

    public:

        Interpolator(double lo, double hi, double v1, double v2) : fRange(lo,hi)
        {
            if (hi != lo) {
                // regular case
                fCoef = (v2-v1)/(hi-lo);
                fOffset = v1 - lo*fCoef;
            } else {
                // degenerate case, avoids division by zero
                fCoef = 0;
                fOffset = (v1+v2)/2;
            }
        }
        double operator()(double v)
        {
            double x = fRange(v);
            return  fOffset + x*fCoef;
        }

        void getLowHigh(double& amin, double& amax)
        {
            amin = fRange.fLo;
            amax = fRange.fHi;
        }
};

//--------------------------------------------------------------------------------------
// Interpolator3pt(lo,mi,hi,v1,vm,v2)
// Map values between lo mid hi to values between v1 vm v2
//--------------------------------------------------------------------------------------
class Interpolator3pt
{

    private:

        Interpolator fSegment1;
        Interpolator fSegment2;
        double fMid;

    public:

        Interpolator3pt(double lo, double mi, double hi, double v1, double vm, double v2) :
            fSegment1(lo, mi, v1, vm),
            fSegment2(mi, hi, vm, v2),
            fMid(mi) {}
        double operator()(double x) { return  (x < fMid) ? fSegment1(x) : fSegment2(x); }

        void getMappingValues(double& amin, double& amid, double& amax)
        {
            fSegment1.getLowHigh(amin, amid);
            fSegment2.getLowHigh(amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Abstract ValueConverter class. Converts values between UI and Faust representations
//--------------------------------------------------------------------------------------
class ValueConverter
{

    public:

        virtual ~ValueConverter() {}
        virtual double ui2faust(double x) = 0;
        virtual double faust2ui(double x) = 0;
};

//--------------------------------------------------------------------------------------
// A converter than can be updated
//--------------------------------------------------------------------------------------

class UpdatableValueConverter : public ValueConverter {
    
    protected:
        
        bool fActive;
        
    public:
        
        UpdatableValueConverter():fActive(true)
        {}
        virtual ~UpdatableValueConverter()
        {}
        
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max) = 0;
        virtual void getMappingValues(double& amin, double& amid, double& amax) = 0;
        
        void setActive(bool on_off) { fActive = on_off; }
        bool getActive() { return fActive; }
    
};


//--------------------------------------------------------------------------------------
// Linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LinearValueConverter : public ValueConverter
{
    
    private:
        
        Interpolator fUI2F;
        Interpolator fF2UI;
        
    public:
        
        LinearValueConverter(double umin, double umax, double fmin, double fmax) :
            fUI2F(umin,umax,fmin,fmax), fF2UI(fmin,fmax,umin,umax)
        {}
        
        LinearValueConverter() : fUI2F(0.,0.,0.,0.), fF2UI(0.,0.,0.,0.)
        {}
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x) { return fF2UI(x); }
    
};

//--------------------------------------------------------------------------------------
// Two segments linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LinearValueConverter2 : public UpdatableValueConverter
{
    
    private:
    
        Interpolator3pt fUI2F;
        Interpolator3pt fF2UI;
        
    public:
    
        LinearValueConverter2(double amin, double amid, double amax, double min, double init, double max) :
            fUI2F(amin, amid, amax, min, init, max), fF2UI(min, init, max, amin, amid, amax)
        {}
        
        LinearValueConverter2() : fUI2F(0.,0.,0.,0.,0.,0.), fF2UI(0.,0.,0.,0.,0.,0.)
        {}
    
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x) { return fF2UI(x); }
    
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max)
        {
            fUI2F = Interpolator3pt(amin, amid, amax, min, init, max);
            fF2UI = Interpolator3pt(min, init, max, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fUI2F.getMappingValues(amin, amid, amax);
        }
    
};

//--------------------------------------------------------------------------------------
// Logarithmic conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class LogValueConverter : public LinearValueConverter
{

    public:

        LogValueConverter(double umin, double umax, double fmin, double fmax) :
            LinearValueConverter(umin, umax, std::log(std::max<double>(DBL_MIN, fmin)), std::log(std::max<double>(DBL_MIN, fmax)))
        {}

        virtual double ui2faust(double x) { return std::exp(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::log(std::max<double>(x, DBL_MIN))); }

};

//--------------------------------------------------------------------------------------
// Exponential conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class ExpValueConverter : public LinearValueConverter
{

    public:

        ExpValueConverter(double umin, double umax, double fmin, double fmax) :
            LinearValueConverter(umin, umax, std::min<double>(DBL_MAX, std::exp(fmin)), std::min<double>(DBL_MAX, std::exp(fmax)))
        {}

        virtual double ui2faust(double x) { return std::log(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::min<double>(DBL_MAX, std::exp(x))); }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up curve (curve 0)
//--------------------------------------------------------------------------------------
class AccUpConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt fA2F;
        Interpolator3pt fF2A;

    public:

        AccUpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmid,fmax),
            fF2A(fmin,fmid,fmax,amin,amid,amax)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmid, fmax);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down curve (curve 1)
//--------------------------------------------------------------------------------------
class AccDownConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator3pt	fF2A;

    public:

        AccDownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmid,fmin),
            fF2A(fmin,fmid,fmax,amax,amid,amin)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
             //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmid, fmin);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amax, amid, amin);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up-Down curve (curve 2)
//--------------------------------------------------------------------------------------
class AccUpDownConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        AccUpDownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmax,fmin),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpDownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmax, fmin);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down-Up curve (curve 3)
//--------------------------------------------------------------------------------------
class AccDownUpConverter : public UpdatableValueConverter
{

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        AccDownUpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmin,fmax),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownUpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmin, fmax);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Base class for ZoneControl
//--------------------------------------------------------------------------------------
class ZoneControl
{

    protected:

        FAUSTFLOAT*	fZone;

    public:

        ZoneControl(FAUSTFLOAT* zone) : fZone(zone) {}
        virtual ~ZoneControl() {}

        virtual void update(double v) {}

        virtual void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max) {}
        virtual void getMappingValues(double& amin, double& amid, double& amax) {}

        FAUSTFLOAT* getZone() { return fZone; }

        virtual void setActive(bool on_off) {}
        virtual bool getActive() { return false; }

        virtual int getCurve() { return -1; }

};

//--------------------------------------------------------------------------------------
//  Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class ConverterZoneControl : public ZoneControl
{

    protected:

        ValueConverter* fValueConverter;

    public:

        ConverterZoneControl(FAUSTFLOAT* zone, ValueConverter* converter) : ZoneControl(zone), fValueConverter(converter) {}
        virtual ~ConverterZoneControl() { delete fValueConverter; } // Assuming fValueConverter is not kept elsewhere...

        virtual void update(double v) { *fZone = fValueConverter->ui2faust(v); }

        ValueConverter* getConverter() { return fValueConverter; }

};

//--------------------------------------------------------------------------------------
// Association of a zone and a four value converter, each one for each possible curve.
// Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class CurveZoneControl : public ZoneControl
{

    private:

        std::vector<UpdatableValueConverter*> fValueConverters;
        int fCurve;

    public:

        CurveZoneControl(FAUSTFLOAT* zone, int curve, double amin, double amid, double amax, double min, double init, double max) : ZoneControl(zone), fCurve(0)
        {
            assert(curve >= 0 && curve <= 3);
            fValueConverters.push_back(new AccUpConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccDownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccUpDownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new AccDownUpConverter(amin, amid, amax, min, init, max));
            fCurve = curve;
        }
        virtual ~CurveZoneControl()
        {
            std::vector<UpdatableValueConverter*>::iterator it;
            for (it = fValueConverters.begin(); it != fValueConverters.end(); it++) {
                delete(*it);
            }
        }
        void update(double v) { if (fValueConverters[fCurve]->getActive()) *fZone = fValueConverters[fCurve]->ui2faust(v); }

        void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max)
        {
            fValueConverters[curve]->setMappingValues(amin, amid, amax, min, init, max);
            fCurve = curve;
        }

        void getMappingValues(double& amin, double& amid, double& amax)
        {
            fValueConverters[fCurve]->getMappingValues(amin, amid, amax);
        }

        void setActive(bool on_off)
        {
            std::vector<UpdatableValueConverter*>::iterator it;
            for (it = fValueConverters.begin(); it != fValueConverters.end(); it++) {
                (*it)->setActive(on_off);
            }
        }

        int getCurve() { return fCurve; }
};

class ZoneReader
{

    private:

        FAUSTFLOAT* fZone;
        Interpolator fInterpolator;

    public:

        ZoneReader(FAUSTFLOAT* zone, double lo, double hi) : fZone(zone), fInterpolator(lo, hi, 0, 255) {}

        virtual ~ZoneReader() {}

        int getValue()
        {
            return (fZone != nullptr) ? int(fInterpolator(*fZone)) : 127;
        }

};

#endif
/**************************  END  ValueConverter.h **************************/
/************************** BEGIN MetaDataUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef MetaData_UI_H
#define MetaData_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <map>
#include <set>
#include <string>
#include <string.h>
#include <assert.h>


static bool startWith(const std::string& str, const std::string& prefix)
{
    return (str.substr(0, prefix.size()) == prefix);
}

/**
 * Convert a dB value into a scale between 0 and 1 (following IEC standard ?)
 */
static FAUSTFLOAT dB2Scale(FAUSTFLOAT dB)
{
    FAUSTFLOAT scale = FAUSTFLOAT(1.0);
    
    /*if (dB < -70.0f)
     scale = 0.0f;
     else*/
    if (dB < FAUSTFLOAT(-60.0))
        scale = (dB + FAUSTFLOAT(70.0)) * FAUSTFLOAT(0.0025);
    else if (dB < FAUSTFLOAT(-50.0))
        scale = (dB + FAUSTFLOAT(60.0)) * FAUSTFLOAT(0.005) + FAUSTFLOAT(0.025);
    else if (dB < FAUSTFLOAT(-40.0))
        scale = (dB + FAUSTFLOAT(50.0)) * FAUSTFLOAT(0.0075) + FAUSTFLOAT(0.075);
    else if (dB < FAUSTFLOAT(-30.0))
        scale = (dB + FAUSTFLOAT(40.0)) * FAUSTFLOAT(0.015) + FAUSTFLOAT(0.15);
    else if (dB < FAUSTFLOAT(-20.0))
        scale = (dB + FAUSTFLOAT(30.0)) * FAUSTFLOAT(0.02) + FAUSTFLOAT(0.3);
    else if (dB < FAUSTFLOAT(-0.001) || dB > FAUSTFLOAT(0.001))  /* if (dB < 0.0) */
        scale = (dB + FAUSTFLOAT(20.0)) * FAUSTFLOAT(0.025) + FAUSTFLOAT(0.5);
    
    return scale;
}

/*******************************************************************************
 * MetaDataUI : Common class for MetaData handling
 ******************************************************************************/

//============================= BEGIN GROUP LABEL METADATA===========================
// Unlike widget's label, metadata inside group's label are not extracted directly by
// the Faust compiler. Therefore they must be extracted within the architecture file
//-----------------------------------------------------------------------------------

class MetaDataUI {
    
    protected:
        
        std::string                         fGroupTooltip;
        std::map<FAUSTFLOAT*, FAUSTFLOAT>   fGuiSize;            // map widget zone with widget size coef
        std::map<FAUSTFLOAT*, std::string>  fTooltip;            // map widget zone with tooltip strings
        std::map<FAUSTFLOAT*, std::string>  fUnit;               // map widget zone to unit string (i.e. "dB")
        std::map<FAUSTFLOAT*, std::string>  fRadioDescription;   // map zone to {'low':440; ...; 'hi':1000.0}
        std::map<FAUSTFLOAT*, std::string>  fMenuDescription;    // map zone to {'low':440; ...; 'hi':1000.0}
        std::set<FAUSTFLOAT*>               fKnobSet;            // set of widget zone to be knobs
        std::set<FAUSTFLOAT*>               fLedSet;             // set of widget zone to be LEDs
        std::set<FAUSTFLOAT*>               fNumSet;             // set of widget zone to be numerical bargraphs
        std::set<FAUSTFLOAT*>               fLogSet;             // set of widget zone having a log UI scale
        std::set<FAUSTFLOAT*>               fExpSet;             // set of widget zone having an exp UI scale
        std::set<FAUSTFLOAT*>               fHiddenSet;          // set of hidden widget zone
        
        void clearMetadata()
        {
            fGuiSize.clear();
            fTooltip.clear();
            fUnit.clear();
            fRadioDescription.clear();
            fMenuDescription.clear();
            fKnobSet.clear();
            fLedSet.clear();
            fNumSet.clear();
            fLogSet.clear();
            fExpSet.clear();
            fHiddenSet.clear();
        }
        
        /**
         * rmWhiteSpaces(): Remove the leading and trailing white spaces of a string
         * (but not those in the middle of the string)
         */
        static std::string rmWhiteSpaces(const std::string& s)
        {
            size_t i = s.find_first_not_of(" \t");
            size_t j = s.find_last_not_of(" \t");
            if ((i != std::string::npos) && (j != std::string::npos)) {
                return s.substr(i, 1+j-i);
            } else {
                return "";
            }
        }
        
        /**
         * Format tooltip string by replacing some white spaces by
         * return characters so that line width doesn't exceed n.
         * Limitation : long words exceeding n are not cut
         */
        std::string formatTooltip(int n, const std::string& tt)
        {
            std::string ss = tt;  // ss string we are going to format
            int lws = 0;          // last white space encountered
            int lri = 0;          // last return inserted
            for (int i = 0; i < (int)tt.size(); i++) {
                if (tt[i] == ' ') lws = i;
                if (((i-lri) >= n) && (lws > lri)) {
                    // insert return here
                    ss[lws] = '\n';
                    lri = lws;
                }
            }
            return ss;
        }
        
    public:
        
        virtual ~MetaDataUI()
        {}
        
        enum Scale {
            kLin,
            kLog,
            kExp
        };
        
        Scale getScale(FAUSTFLOAT* zone)
        {
            if (fLogSet.count(zone) > 0) return kLog;
            if (fExpSet.count(zone) > 0) return kExp;
            return kLin;
        }
        
        bool isKnob(FAUSTFLOAT* zone)
        {
            return fKnobSet.count(zone) > 0;
        }
        
        bool isRadio(FAUSTFLOAT* zone)
        {
            return fRadioDescription.count(zone) > 0;
        }
        
        bool isMenu(FAUSTFLOAT* zone)
        {
            return fMenuDescription.count(zone) > 0;
        }
        
        bool isLed(FAUSTFLOAT* zone)
        {
            return fLedSet.count(zone) > 0;
        }
        
        bool isNumerical(FAUSTFLOAT* zone)
        {
            return fNumSet.count(zone) > 0;
        }
        
        bool isHidden(FAUSTFLOAT* zone)
        {
            return fHiddenSet.count(zone) > 0;
        }
        
        /**
         * Extracts metadata from a label : 'vol [unit: dB]' -> 'vol' + metadata(unit=dB)
         */
        static void extractMetadata(const std::string& fulllabel, std::string& label, std::map<std::string, std::string>& metadata)
        {
            enum {kLabel, kEscape1, kEscape2, kEscape3, kKey, kValue};
            int state = kLabel; int deep = 0;
            std::string key, value;
            
            for (unsigned int i = 0; i < fulllabel.size(); i++) {
                char c = fulllabel[i];
                switch (state) {
                    case kLabel :
                        assert(deep == 0);
                        switch (c) {
                            case '\\' : state = kEscape1; break;
                            case '[' : state = kKey; deep++; break;
                            default : label += c;
                        }
                        break;
                        
                    case kEscape1:
                        label += c;
                        state = kLabel;
                        break;
                        
                    case kEscape2:
                        key += c;
                        state = kKey;
                        break;
                        
                    case kEscape3:
                        value += c;
                        state = kValue;
                        break;
                        
                    case kKey:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape2;
                                break;
                                
                            case '[':
                                deep++;
                                key += c;
                                break;
                                
                            case ':':
                                if (deep == 1) {
                                    state = kValue;
                                } else {
                                    key += c;
                                }
                                break;
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = "";
                                    state = kLabel;
                                    key="";
                                    value="";
                                } else {
                                    key += c;
                                }
                                break;
                            default : key += c;
                        }
                        break;
                        
                    case kValue:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape3;
                                break;
                                
                            case '[':
                                deep++;
                                value += c;
                                break;
                                
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = rmWhiteSpaces(value);
                                    state = kLabel;
                                    key = "";
                                    value = "";
                                } else {
                                    value += c;
                                }
                                break;
                            default : value += c;
                        }
                        break;
                        
                    default:
                        std::cerr << "ERROR unrecognized state " << state << std::endl;
                }
            }
            label = rmWhiteSpaces(label);
        }
        
        /**
         * Analyses the widget zone metadata declarations and takes appropriate actions
         */
        void declare(FAUSTFLOAT* zone, const char* key, const char* value)
        {
            if (zone == 0) {
                // special zone 0 means group metadata
                if (strcmp(key, "tooltip") == 0) {
                    // only group tooltip are currently implemented
                    fGroupTooltip = formatTooltip(30, value);
                } else if (strcmp(key, "hidden") == 0) {
                    fHiddenSet.insert(zone);
                }
            } else {
                if (strcmp(key, "size") == 0) {
                    fGuiSize[zone] = atof(value);
                }
                else if (strcmp(key, "tooltip") == 0) {
                    fTooltip[zone] = formatTooltip(30, value);
                }
                else if (strcmp(key, "unit") == 0) {
                    fUnit[zone] = value;
                }
                else if (strcmp(key, "hidden") == 0) {
                    fHiddenSet.insert(zone);
                }
                else if (strcmp(key, "scale") == 0) {
                    if (strcmp(value, "log") == 0) {
                        fLogSet.insert(zone);
                    } else if (strcmp(value, "exp") == 0) {
                        fExpSet.insert(zone);
                    }
                }
                else if (strcmp(key, "style") == 0) {
                    if (strcmp(value, "knob") == 0) {
                        fKnobSet.insert(zone);
                    } else if (strcmp(value, "led") == 0) {
                        fLedSet.insert(zone);
                    } else if (strcmp(value, "numerical") == 0) {
                        fNumSet.insert(zone);
                    } else {
                        const char* p = value;
                        if (parseWord(p, "radio")) {
                            fRadioDescription[zone] = std::string(p);
                        } else if (parseWord(p, "menu")) {
                            fMenuDescription[zone] = std::string(p);
                        }
                    }
                }
            }
        }
    
};

#endif
/**************************  END  MetaDataUI.h **************************/
/************************** BEGIN ring-buffer.h **************************/
/*
  Copyright (C) 2000 Paul Davis
  Copyright (C) 2003 Rohan Drape
  Copyright (C) 2016 GRAME (renaming for internal use)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

  ISO/POSIX C version of Paul Davis's lock free ringbuffer C++ code.
  This is safe for the case of one read thread and one write thread.
*/

#ifndef __ring_buffer__
#define __ring_buffer__

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
# pragma warning (disable: 4334)
#else
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

typedef struct {
    char *buf;
    size_t len;
}
ringbuffer_data_t;

typedef struct {
    char *buf;
    volatile size_t write_ptr;
    volatile size_t read_ptr;
    size_t	size;
    size_t	size_mask;
    int	mlocked;
}
ringbuffer_t;

static ringbuffer_t *ringbuffer_create(size_t sz);
static void ringbuffer_free(ringbuffer_t *rb);
static void ringbuffer_get_read_vector(const ringbuffer_t *rb,
                                         ringbuffer_data_t *vec);
static void ringbuffer_get_write_vector(const ringbuffer_t *rb,
                                          ringbuffer_data_t *vec);
static size_t ringbuffer_read(ringbuffer_t *rb, char *dest, size_t cnt);
static size_t ringbuffer_peek(ringbuffer_t *rb, char *dest, size_t cnt);
static void ringbuffer_read_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_read_space(const ringbuffer_t *rb);
static int ringbuffer_mlock(ringbuffer_t *rb);
static void ringbuffer_reset(ringbuffer_t *rb);
static void ringbuffer_reset_size (ringbuffer_t * rb, size_t sz);
static size_t ringbuffer_write(ringbuffer_t *rb, const char *src,
                                 size_t cnt);
static void ringbuffer_write_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_write_space(const ringbuffer_t *rb);

/* Create a new ringbuffer to hold at least `sz' bytes of data. The
   actual buffer size is rounded up to the next power of two. */

static ringbuffer_t *
ringbuffer_create (size_t sz)
{
	size_t power_of_two;
	ringbuffer_t *rb;

	if ((rb = (ringbuffer_t *) malloc (sizeof (ringbuffer_t))) == NULL) {
		return NULL;
	}

	for (power_of_two = 1u; 1u << power_of_two < sz; power_of_two++);

	rb->size = 1u << power_of_two;
	rb->size_mask = rb->size;
	rb->size_mask -= 1;
	rb->write_ptr = 0;
	rb->read_ptr = 0;
	if ((rb->buf = (char *) malloc (rb->size)) == NULL) {
		free (rb);
		return NULL;
	}
	rb->mlocked = 0;

	return rb;
}

/* Free all data associated with the ringbuffer `rb'. */

static void
ringbuffer_free (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (rb->mlocked) {
		munlock (rb->buf, rb->size);
	}
#endif /* USE_MLOCK */
	free (rb->buf);
	free (rb);
}

/* Lock the data block of `rb' using the system call 'mlock'.  */

static int
ringbuffer_mlock (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (mlock (rb->buf, rb->size)) {
		return -1;
	}
#endif /* USE_MLOCK */
	rb->mlocked = 1;
	return 0;
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset (ringbuffer_t * rb)
{
	rb->read_ptr = 0;
	rb->write_ptr = 0;
    memset(rb->buf, 0, rb->size);
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset_size (ringbuffer_t * rb, size_t sz)
{
    rb->size = sz;
    rb->size_mask = rb->size;
    rb->size_mask -= 1;
    rb->read_ptr = 0;
    rb->write_ptr = 0;
}

/* Return the number of bytes available for reading. This is the
   number of bytes in front of the read pointer and behind the write
   pointer.  */

static size_t
ringbuffer_read_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return w - r;
	} else {
		return (w - r + rb->size) & rb->size_mask;
	}
}

/* Return the number of bytes available for writing. This is the
   number of bytes in front of the write pointer and behind the read
   pointer.  */

static size_t
ringbuffer_write_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		return (r - w) - 1;
	} else {
		return rb->size - 1;
	}
}

/* The copying data reader. Copy at most `cnt' bytes from `rb' to
   `dest'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_read (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[rb->read_ptr]), n1);
	rb->read_ptr = (rb->read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[rb->read_ptr]), n2);
		rb->read_ptr = (rb->read_ptr + n2) & rb->size_mask;
	}

	return to_read;
}

/* The copying data reader w/o read pointer advance. Copy at most
   `cnt' bytes from `rb' to `dest'.  Returns the actual number of bytes
   copied. */

static size_t
ringbuffer_peek (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;
	size_t tmp_read_ptr;

	tmp_read_ptr = rb->read_ptr;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = tmp_read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - tmp_read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[tmp_read_ptr]), n1);
	tmp_read_ptr = (tmp_read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[tmp_read_ptr]), n2);
	}

	return to_read;
}

/* The copying data writer. Copy at most `cnt' bytes to `rb' from
   `src'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_write (ringbuffer_t * rb, const char *src, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_write;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_write_space (rb)) == 0) {
		return 0;
	}

	to_write = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->write_ptr + to_write;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->write_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_write;
		n2 = 0;
	}

	memcpy (&(rb->buf[rb->write_ptr]), src, n1);
	rb->write_ptr = (rb->write_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (&(rb->buf[rb->write_ptr]), src + n1, n2);
		rb->write_ptr = (rb->write_ptr + n2) & rb->size_mask;
	}

	return to_write;
}

/* Advance the read pointer `cnt' places. */

static void
ringbuffer_read_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->read_ptr + cnt) & rb->size_mask;
	rb->read_ptr = tmp;
}

/* Advance the write pointer `cnt' places. */

static void
ringbuffer_write_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->write_ptr + cnt) & rb->size_mask;
	rb->write_ptr = tmp;
}

/* The non-copying data reader. `vec' is an array of two places. Set
   the values at `vec' to hold the current readable data at `rb'. If
   the readable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_read_vector (const ringbuffer_t * rb,
				 ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = w - r;
	} else {
		free_cnt = (w - r + rb->size) & rb->size_mask;
	}

	cnt2 = r + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = rb->size - r;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;

	} else {

		/* Single part vector: just the rest of the buffer */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

/* The non-copying data writer. `vec' is an array of two places. Set
   the values at `vec' to hold the current writeable data at `rb'. If
   the writeable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_write_vector (const ringbuffer_t * rb,
				  ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		free_cnt = (r - w) - 1;
	} else {
		free_cnt = rb->size - 1;
	}

	cnt2 = w + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[w]);
		vec[0].len = rb->size - w;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;
	} else {
		vec[0].buf = &(rb->buf[w]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

#endif // __ring_buffer__
/**************************  END  ring-buffer.h **************************/

/*******************************************************************************
 * GUI : Abstract Graphic User Interface
 * Provides additional mechanisms to synchronize widgets and zones. Widgets
 * should both reflect the value of a zone and allow to change this value.
 ******************************************************************************/

class uiItem;
class GUI;
struct clist;

typedef void (*uiCallback)(FAUSTFLOAT val, void* data);

struct uiItemBase
{
    
    uiItemBase(GUI* ui, FAUSTFLOAT* zone)
    {
        assert(ui);
        assert(zone);
    }
    
    virtual ~uiItemBase()
    {}
    
    virtual void modifyZone(FAUSTFLOAT v) = 0;
    virtual void modifyZone(double date, FAUSTFLOAT v) {}
    virtual double cache() = 0;
    virtual void reflectZone() = 0;
};

// Declared as 'static' to avoid code duplication at link time
static void deleteClist(clist* cl);

struct clist : public std::list<uiItemBase*>
{
    
    virtual ~clist()
    {
        deleteClist(this);
    }
        
};

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data);

typedef std::map<FAUSTFLOAT*, clist*> zmap;

typedef std::map<FAUSTFLOAT*, ringbuffer_t*> ztimedmap;

class GUI : public UI
{
		
    private:
     
        static std::list<GUI*> fGuiList;
        zmap fZoneMap;
        bool fStopped;
        
     public:
            
        GUI():fStopped(false)
        {	
            fGuiList.push_back(this);
        }
        
        virtual ~GUI() 
        {   
            // delete all items
            for (auto& it : fZoneMap) {
                delete it.second;
            }
            // suppress 'this' in static fGuiList
            fGuiList.remove(this);
        }

        // -- registerZone(z,c) : zone management
        
        void registerZone(FAUSTFLOAT* z, uiItemBase* c)
        {
            if (fZoneMap.find(z) == fZoneMap.end()) fZoneMap[z] = new clist();
            fZoneMap[z]->push_back(c);
        }
 
        void updateZone(FAUSTFLOAT* z)
        {
            FAUSTFLOAT v = *z;
            clist* cl = fZoneMap[z];
            for (auto& c : *cl) {
                if (c->cache() != v) c->reflectZone();
            }
        }
    
        void updateAllZones()
        {
            for (auto& m : fZoneMap) {
                updateZone(m.first);
            }
        }
    
        static void updateAllGuis()
        {
            for (auto& g : fGuiList) {
                g->updateAllZones();
            }
        }
    
        static void runAllGuis()
        {
            for (auto& g : fGuiList) {
                g->run();
            }
        }
    
        void addCallback(FAUSTFLOAT* zone, uiCallback foo, void* data)
        {
            createUiCallbackItem(this, zone, foo, data);
        }

        virtual void show() {};	
        virtual bool run() { return false; };

        virtual void stop() { fStopped = true; }
        bool stopped() { return fStopped; }
    
        // -- widget's layouts
        
        virtual void openTabBox(const char* label) {};
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
    
        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
    
        // Static global for timed zones, shared between all UI that will set timed values
        static ztimedmap gTimedZoneMap;

};

/**
 * User Interface Item: abstract definition
 */

template <typename REAL>
class uiTypedItem : public uiItemBase
{
    protected:
        
        GUI* fGUI;
        REAL* fZone;
        REAL fCache;
        
        uiTypedItem(GUI* ui, REAL* zone):uiItemBase(ui, static_cast<FAUSTFLOAT*>(zone)),
        fGUI(ui), fZone(zone), fCache(REAL(-123456.654321))
        {
            ui->registerZone(zone, this);
        }
        
    public:
        
        virtual ~uiTypedItem()
        {}
    
        void modifyZone(REAL v)
        {
            fCache = v;
            if (*fZone != v) {
                *fZone = v;
                fGUI->updateZone(fZone);
            }
        }
    
        double cache() { return fCache; }
    
};

class uiItem : public uiTypedItem<FAUSTFLOAT> {
    
    protected:
    
        uiItem(GUI* ui, FAUSTFLOAT* zone):uiTypedItem<FAUSTFLOAT>(ui, zone)
        {}

    public:

        virtual ~uiItem() 
        {}

		void modifyZone(FAUSTFLOAT v)
		{
			fCache = v;
			if (*fZone != v) {
				*fZone = v;
				fGUI->updateZone(fZone);
			}
		}

};

/**
 * Base class for items with a converter
 */

struct uiConverter {
    
    ValueConverter* fConverter;
    
    uiConverter(MetaDataUI::Scale scale, FAUSTFLOAT umin, FAUSTFLOAT umax, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        // Select appropriate converter according to scale mode
        if (scale == MetaDataUI::kLog) {
            fConverter = new LogValueConverter(umin, umax, fmin, fmax);
        } else if (scale == MetaDataUI::kExp) {
            fConverter = new ExpValueConverter(umin, umax, fmin, fmax);
        } else {
            fConverter = new LinearValueConverter(umin, umax, fmin, fmax);
        }
    }
    
    virtual ~uiConverter()
    {
        delete fConverter;
    }
};

/**
 * User Interface item owned (and so deleted) by external code
 */

class uiOwnedItem : public uiItem {
    
    protected:
    
        uiOwnedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
    
     public:
    
        virtual ~uiOwnedItem()
        {}
    
        virtual void reflectZone() {}
};

/**
 * Callback Item
 */

class uiCallbackItem : public uiItem {
    
    protected:
    
        uiCallback fCallback;
        void* fData;
    
    public:
    
        uiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
        : uiItem(ui, zone), fCallback(foo), fData(data) {}
        
        virtual void reflectZone() 
        {		
            FAUSTFLOAT v = *fZone;
            fCache = v; 
            fCallback(v, fData);	
        }
};

/**
 *  For timestamped control
 */

struct DatedControl {
    
    double fDate;
    FAUSTFLOAT fValue;
    
    DatedControl(double d = 0., FAUSTFLOAT v = FAUSTFLOAT(0)):fDate(d), fValue(v) {}
    
};

/**
 * Base class for timed items
 */

class uiTimedItem : public uiItem
{
    
    protected:
        
        bool fDelete;
        
    public:
    
        using uiItem::modifyZone;
        
        uiTimedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {
            if (GUI::gTimedZoneMap.find(fZone) == GUI::gTimedZoneMap.end()) {
                GUI::gTimedZoneMap[fZone] = ringbuffer_create(8192);
                fDelete = true;
            } else {
                fDelete = false;
            }
        }
        
        virtual ~uiTimedItem()
        {
            ztimedmap::iterator it;
            if (fDelete && ((it = GUI::gTimedZoneMap.find(fZone)) != GUI::gTimedZoneMap.end())) {
                ringbuffer_free((*it).second);
                GUI::gTimedZoneMap.erase(it);
            }
        }
        
        virtual void modifyZone(double date, FAUSTFLOAT v)
        {
            size_t res;
            DatedControl dated_val(date, v);
            if ((res = ringbuffer_write(GUI::gTimedZoneMap[fZone], (const char*)&dated_val, sizeof(DatedControl))) != sizeof(DatedControl)) {
                std::cerr << "ringbuffer_write error DatedControl" << std::endl;
            }
        }
    
};

/**
 * Allows to group a set of zones
 */

class uiGroupItem : public uiItem
{
    protected:
    
        std::vector<FAUSTFLOAT*> fZoneMap;

    public:
    
        uiGroupItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
        virtual ~uiGroupItem() 
        {}
        
        virtual void reflectZone() 
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            
            // Update all zones of the same group
            for (auto& it : fZoneMap) {
                *it = v;
            }
        }
        
        void addZone(FAUSTFLOAT* zone) { fZoneMap.push_back(zone); }

};

// Can not be defined as method in the classes

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
{
    new uiCallbackItem(ui, zone, foo, data);
}

static void deleteClist(clist* cl)
{
    for (auto& it : *cl) {
        uiOwnedItem* owned = dynamic_cast<uiOwnedItem*>(it);
        // owned items are deleted by external code
        if (!owned) {
            delete it;
        }
    }
}

#endif
/**************************  END  GUI.h **************************/
/*

  Copyright (C) 2011 Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __RootNode__
#define __RootNode__

#include <map>
#include <string>
#include <vector>

/************************** BEGIN JSONUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef FAUST_JSONUI_H
#define FAUST_JSONUI_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>


/*******************************************************************************
 * JSONUI : Faust User Interface
 * This class produce a complete JSON decription of the DSP instance.
 ******************************************************************************/

template <typename REAL>
class JSONUIReal : public PathBuilder, public Meta, public UIReal<REAL>
{

    protected:
    
        std::stringstream fUI;
        std::stringstream fMeta;
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        std::string fVersion;           // Compiler version
        std::string fCompileOptions;    // Compilation options
        std::vector<std::string> fLibraryList;
        std::vector<std::string> fIncludePathnames;
        std::string fName;
        std::string fFileName;
        std::string fExpandedCode;
        std::string fSHAKey;
        int fDSPSize;                   // In bytes
        std::map<std::string, int> fPathTable;
        bool fExtended;
    
        char fCloseUIPar;
        char fCloseMetaPar;
        int fTab;
    
        int fInputs, fOutputs, fSRIndex;
         
        void tab(int n, std::ostream& fout)
        {
            fout << '\n';
            while (n-- > 0) {
                fout << '\t';
            }
        }
    
        std::string flatten(const std::string& src)
        {
            std::string dst;
            for (size_t i = 0; i < src.size(); i++) {
                switch (src[i]) {
                    case '\n':
                    case '\t':
                        break;
                    default:
                        dst += src[i];
                        break;
                }
            }
            return dst;
        }
    
        void addMeta(int tab_val, bool quote = true)
        {
            if (fMetaAux.size() > 0) {
                tab(tab_val, fUI); fUI << "\"meta\": [";
                std::string sep = "";
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    fUI << sep;
                    tab(tab_val + 1, fUI); fUI << "{ \"" << fMetaAux[i].first << "\": \"" << fMetaAux[i].second << "\" }";
                    sep = ",";
                }
                tab(tab_val, fUI); fUI << ((quote) ? "],": "]");
                fMetaAux.clear();
            }
        }
    
        int getAddressIndex(const std::string& path)
        {
            return (fPathTable.find(path) != fPathTable.end()) ? fPathTable[path] : -1;
        }
      
     public:
     
        JSONUIReal(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const std::map<std::string, int>& path_table)
        {
            init(name, filename, inputs, outputs, sr_index, sha_key, dsp_code, version, compile_options, library_list, include_pathnames, size, path_table);
        }

        JSONUIReal(const std::string& name, const std::string& filename, int inputs, int outputs)
        {
            init(name, filename, inputs, outputs, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }

        JSONUIReal(int inputs, int outputs)
        {
            init("", "", inputs, outputs, -1, "", "","", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }
        
        JSONUIReal()
        {
            init("", "", -1, -1, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, std::map<std::string, int>());
        }
 
        virtual ~JSONUIReal() {}
        
        void setInputs(int inputs) { fInputs = inputs; }
        void setOutputs(int outputs) { fOutputs = outputs; }
    
        void setSRIndex(int sr_index) { fSRIndex = sr_index; }
    
        // Init may be called multiple times so fMeta and fUI are reinitialized
        void init(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const std::map<std::string, int>& path_table,
                  bool extended = false)
        {
            fTab = 1;
            fExtended = extended;
            if (fExtended) {
                fUI << std::setprecision(std::numeric_limits<REAL>::max_digits10);
                fMeta << std::setprecision(std::numeric_limits<REAL>::max_digits10);
            }
            
            // Start Meta generation
            fMeta.str("");
            tab(fTab, fMeta); fMeta << "\"meta\": [";
            fCloseMetaPar = ' ';
            
            // Start UI generation
            fUI.str("");
            tab(fTab, fUI); fUI << "\"ui\": [";
            fCloseUIPar = ' ';
            fTab += 1;
            
            fName = name;
            fFileName = filename;
            fInputs = inputs;
            fOutputs = outputs;
            fSRIndex = sr_index;
            fExpandedCode = dsp_code;
            fSHAKey = sha_key;
            fDSPSize = size;
            fPathTable = path_table;
            fVersion = version;
            fCompileOptions = compile_options;
            fLibraryList = library_list;
            fIncludePathnames = include_pathnames;
        }
   
        // -- widget's layouts
    
        virtual void openGenericGroup(const char* label, const char* name)
        {
            pushLabel(label);
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"items\": [";
            fCloseUIPar = ' ';
            fTab += 1;
        }

        virtual void openTabBox(const char* label)
        {
            openGenericGroup(label, "tgroup");
        }
    
        virtual void openHorizontalBox(const char* label)
        {
            openGenericGroup(label, "hgroup");
        }
    
        virtual void openVerticalBox(const char* label)
        {
            openGenericGroup(label, "vgroup");
        }
    
        virtual void closeBox()
        {
            popLabel();
            fTab -= 1;
            tab(fTab, fUI); fUI << "]";
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        // -- active widgets
  
        virtual void addGenericButton(const char* label, const char* name)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ((fMetaAux.size() > 0) ? "," : "");
            } else {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fMetaAux.size() > 0) ? "," : "");
            }
            addMeta(fTab, false);
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "button");
        }
    
        virtual void addCheckButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "checkbox");
        }

        virtual void addGenericEntry(const char* label, const char* name, REAL init, REAL min, REAL max, REAL step)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"init\": " << init << ",";
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max << ",";
            tab(fTab, fUI); fUI << "\"step\": " << step;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "vslider", init, min, max, step);
        }
    
        virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "hslider", init, min, max, step);
        }
    
        virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericEntry(label, "nentry", init, min, max, step);
        }

        // -- passive widgets
    
        virtual void addGenericBargraph(const char* label, const char* name, REAL min, REAL max) 
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) 
        {
            addGenericBargraph(label, "hbargraph", min, max);
        }
    
        virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max)
        {
            addGenericBargraph(label, "vbargraph", min, max);
        }
    
        virtual void addSoundfile(const char* label, const char* url, Soundfile** zone)
        {
            std::string path = buildPath(label);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << "soundfile" << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\"" << ",";
            tab(fTab, fUI); fUI << "\"url\": \"" << url << "\"" << ",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fPathTable.size() > 0) ? "," : "");
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path);
            }
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        // -- metadata declarations

        virtual void declare(REAL* zone, const char* key, const char* val)
        {
            fMetaAux.push_back(std::make_pair(key, val));
        }
    
        // Meta interface
        virtual void declare(const char* key, const char* value)
        {
            fMeta << fCloseMetaPar;
            // fName found in metadata
            if ((strcmp(key, "name") == 0) && (fName == "")) fName = value;
            // fFileName found in metadata
            if ((strcmp(key, "filename") == 0) && (fFileName == "")) fFileName = value;
            tab(fTab, fMeta); fMeta << "{ " << "\"" << key << "\"" << ": " << "\"" << value << "\" }";
            fCloseMetaPar = ',';
        }
    
        std::string JSON(bool flat = false)
        {
            fTab = 0;
            std::stringstream JSON;
            if (fExtended) {
                JSON << std::setprecision(std::numeric_limits<REAL>::max_digits10);
            }
            JSON << "{";
            fTab += 1;
            tab(fTab, JSON); JSON << "\"name\": \"" << fName << "\",";
            tab(fTab, JSON); JSON << "\"filename\": \"" << fFileName << "\",";
            if (fVersion != "") { tab(fTab, JSON); JSON << "\"version\": \"" << fVersion << "\","; }
            if (fCompileOptions != "") { tab(fTab, JSON); JSON << "\"compile_options\": \"" <<  fCompileOptions << "\","; }
            if (fLibraryList.size() > 0) {
                tab(fTab, JSON);
                JSON << "\"library_list\": [";
                for (size_t i = 0; i < fLibraryList.size(); i++) {
                    JSON << "\"" << fLibraryList[i] << "\"";
                    if (i < (fLibraryList.size() - 1)) JSON << ",";
                }
                JSON << "],";
            }
            if (fIncludePathnames.size() > 0) {
                tab(fTab, JSON);
                JSON << "\"include_pathnames\": [";
                for (size_t i = 0; i < fIncludePathnames.size(); i++) {
                    JSON << "\"" << fIncludePathnames[i] << "\"";
                    if (i < (fIncludePathnames.size() - 1)) JSON << ",";
                }
                JSON << "],";
            }
            if (fDSPSize != -1) { tab(fTab, JSON); JSON << "\"size\": " << fDSPSize << ","; }
            if (fSHAKey != "") { tab(fTab, JSON); JSON << "\"sha_key\": \"" << fSHAKey << "\","; }
            if (fExpandedCode != "") { tab(fTab, JSON); JSON << "\"code\": \"" << fExpandedCode << "\","; }
            tab(fTab, JSON); JSON << "\"inputs\": " << fInputs << ",";
            tab(fTab, JSON); JSON << "\"outputs\": " << fOutputs << ",";
            if (fSRIndex != -1) { tab(fTab, JSON); JSON << "\"sr_index\": " << fSRIndex << ","; }
            tab(fTab, fMeta); fMeta << "],";
            tab(fTab, fUI); fUI << "]";
            fTab -= 1;
            if (fCloseMetaPar == ',') { // If "declare" has been called, fCloseMetaPar state is now ','
                JSON << fMeta.str() << fUI.str();
            } else {
                JSON << fUI.str();
            }
            tab(fTab, JSON); JSON << "}";
            return (flat) ? flatten(JSON.str()) : JSON.str();
        }
    
};

// Externally available class using FAUSTFLOAT

struct JSONUI : public JSONUIReal<FAUSTFLOAT>, public UI
{
    
    JSONUI(const std::string& name,
           const std::string& filename,
           int inputs,
           int outputs,
           int sr_index,
           const std::string& sha_key,
           const std::string& dsp_code,
           const std::string& version,
           const std::string& compile_options,
           const std::vector<std::string>& library_list,
           const std::vector<std::string>& include_pathnames,
           int size,
           const std::map<std::string, int>& path_table):
    JSONUIReal<FAUSTFLOAT>(name, filename,
                          inputs, outputs,
                          sr_index,
                          sha_key, dsp_code,
                          version, compile_options,
                          library_list, include_pathnames,
                          size, path_table)
    {}
    
    JSONUI(const std::string& name, const std::string& filename, int inputs, int outputs):
    JSONUIReal<FAUSTFLOAT>(name, filename, inputs, outputs)
    {}
    
    JSONUI(int inputs, int outputs):JSONUIReal<FAUSTFLOAT>(inputs, outputs)
    {}
    
    JSONUI():JSONUIReal<FAUSTFLOAT>()
    {}

    virtual void openTabBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openTabBox(label);
    }
    virtual void openHorizontalBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openHorizontalBox(label);
    }
    virtual void openVerticalBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openVerticalBox(label);
    }
    virtual void closeBox()
    {
        JSONUIReal<FAUSTFLOAT>::closeBox();
    }
    
    // -- active widgets
    
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addButton(label, zone);
    }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addCheckButton(label, zone);
    }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalSlider(label, zone, init, min, max, step);
    }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalSlider(label, zone, init, min, max, step);
    }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addNumEntry(label, zone, init, min, max, step);
    }
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalBargraph(label, zone, min, max);
    }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalBargraph(label, zone, min, max);
    }
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone)
    {
        JSONUIReal<FAUSTFLOAT>::addSoundfile(label, filename, sf_zone);
    }
    
    // -- metadata declarations
    
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
    {
        JSONUIReal<FAUSTFLOAT>::declare(zone, key, val);
    }

    virtual void declare(const char* key, const char* val)
    {
        JSONUIReal<FAUSTFLOAT>::declare(key, val);
    }

    virtual ~JSONUI() {}
    
};

#endif // FAUST_JSONUI_H
/**************************  END  JSONUI.h **************************/

namespace oscfaust
{

class OSCIO;
class RootNode;
typedef class SMARTP<RootNode> SRootNode;

/**
 * an alias target includes a map to rescale input values to output values
 * and a target osc address. The input values can be given in reversed order
 * to reverse the control
 */
struct aliastarget
{
	double      fMinIn;
	double      fMaxIn;
	double      fMinOut;
	double      fMaxOut;
	std::string fTarget;	// the real osc address

	aliastarget(const char* address, double imin, double imax, double omin, double omax)
		: fMinIn(imin), fMaxIn(imax), fMinOut(omin), fMaxOut(omax), fTarget(address) {}

	aliastarget(const aliastarget& t)
		: fMinIn(t.fMinIn), fMaxIn(t.fMaxIn), fMinOut(t.fMinOut), fMaxOut(t.fMaxOut), fTarget(t.fTarget) {}

	double scale(double x) const 
    {
        if (fMinIn < fMaxIn) {
            // increasing control
            double z = (x < fMinIn) ? fMinIn : (x > fMaxIn) ? fMaxIn : x;
            return fMinOut + (z-fMinIn)*(fMaxOut-fMinOut)/(fMaxIn-fMinIn);
            
        } else if (fMinIn > fMaxIn) {
            // reversed control
            double z = (x < fMaxIn) ? fMaxIn : (x > fMinIn) ? fMinIn : x;
            return fMinOut + (fMinIn-z)*(fMaxOut-fMinOut)/(fMinIn-fMaxIn);
            
        } else {
            // no control !
            return (fMinOut+fMaxOut)/2.0f;
        }
    }
    
    double invscale(double x) const
    {
        if (fMinOut < fMaxOut) {
            // increasing control
            double z = (x < fMinOut) ? fMinOut : (x > fMaxOut) ? fMaxOut : x;
            return fMinIn + (z-fMinOut)*(fMaxIn-fMinIn)/(fMaxOut-fMinOut);
            
        } else if (fMinOut > fMaxOut) {
            // reversed control
            double z = (x < fMaxOut) ? fMaxOut : (x > fMinOut) ? fMinOut : x;
            return fMinIn + (fMinOut-z)*(fMaxIn-fMinIn)/(fMinOut-fMaxOut);
            
        } else {
            // no control !
            return (fMinIn+fMaxIn)/2.0f;
        }
    }
};

//--------------------------------------------------------------------------
/*!
	\brief a faust root node

	A Faust root node handles the \c 'hello' message and provides support
	for incoming osc signal data. 
*/
class RootNode : public MessageDriven
{

    private:
        int *fUPDIn, *fUDPOut, *fUDPErr;    // the osc port numbers (required by the hello method)
        OSCIO* fIO;                         // an OSC IO controler
        JSONUI* fJSON;

        typedef std::map<std::string, std::vector<aliastarget> > TAliasMap;
        TAliasMap fAliases;

        template <typename T>
        void processAliasAux(const std::string& address, T val);
        void processAlias(const std::string& address, float val);
        void processAlias(const std::string& address, double val);
    
        void eraseAliases(const std::string& target);
        void eraseAlias(const std::string& target, const std::string& alias);
        bool aliasError(const Message* msg);

    protected:
        RootNode(const char *name, JSONUI* json, OSCIO* io = NULL) : MessageDriven(name, ""), fUPDIn(0), fUDPOut(0), fUDPErr(0), fIO(io), fJSON(json) {}
        virtual ~RootNode() {}

    public:
        static SRootNode create(const char* name, JSONUI* json, OSCIO* io = NULL) { return new RootNode(name, json, io); }

        virtual void processMessage(const Message* msg);
        virtual bool accept(const Message* msg);
        virtual void get(unsigned long ipdest) const;
        virtual void get(unsigned long ipdest, const std::string& what) const;

        template <typename T>
        bool aliasMsgAux(const Message* msg, T omin, T omax);
        bool aliasMsg(const Message* msg, double omin, double omax);
        bool aliasMsg(const Message* msg, float omin, float omax);
    
        template <typename T>
        void addAliasAux(const char* alias, const char* address, T imin, T imax, T omin, T omax);
        void addAlias(const char* alias, const char* address, float imin, float imax, float omin, float omax);
        void addAlias(const char* alias, const char* address, double imin, double imax, double omin, double omax);
    
        bool acceptSignal(const Message* msg);      ///< handler for signal data
        void hello(unsigned long ipdest) const;     ///< handler for the 'hello' message
        void setPorts(int* in, int* out, int* err);

        std::vector<std::pair<std::string, double> > getAliases(const std::string& address, double value);
};

} // end namespoace

#endif

namespace oscfaust
{

/**
 * map (rescale) input values to output values
 */
template <typename C> struct mapping
{
	const C fMinOut;
	const C fMaxOut;
	mapping(C omin, C omax) : fMinOut(omin), fMaxOut(omax) {}
	C clip (C x) { return (x < fMinOut) ? fMinOut : (x > fMaxOut) ? fMaxOut : x; }
};

//--------------------------------------------------------------------------
/*!
	\brief a faust node is a terminal node and represents a faust parameter controler
*/
template <typename C> class FaustNode : public MessageDriven, public uiTypedItem<C>
{
	mapping<C>	fMapping;
    RootNode*   fRoot;
    bool        fInput;  // true for input nodes (slider, button...)
	
	//---------------------------------------------------------------------
	// Warning !!!
	// The cast (C*)fZone is necessary because the real size allocated is
	// only known at execution time. When the library is compiled, fZone is
	// uniquely defined by FAUSTFLOAT.
	//---------------------------------------------------------------------
	bool store(C val) { *(C*)this->fZone = fMapping.clip(val); return true; }
	void sendOSC() const;

	protected:
		FaustNode(RootNode* root, const char *name, C* zone, C init, C min, C max, const char* prefix, GUI* ui, bool initZone, bool input) 
			: MessageDriven(name, prefix), uiTypedItem<C>(ui, zone), fMapping(min, max), fRoot(root), fInput(input)
			{
                if (initZone) {
                    *zone = init; 
                }
            }
			
		virtual ~FaustNode() {}

	public:
		typedef SMARTP<FaustNode<C> > SFaustNode;
		static SFaustNode create(RootNode* root, const char* name, C* zone, C init, C min, C max, const char* prefix, GUI* ui, bool initZone, bool input)	
        { 
            SFaustNode node = new FaustNode(root, name, zone, init, min, max, prefix, ui, initZone, input); 
            /*
                Since FaustNode is a subclass of uiItem, the pointer will also be kept in the GUI class, and it's desallocation will be done there.
                So we don't want to have smartpointer logic desallocate it and we increment the refcount.
            */
            node->addReference();
            return node; 
        }
    
		bool accept(const Message* msg);
		void get(unsigned long ipdest) const;		///< handler for the 'get' message
		virtual void reflectZone() { sendOSC(); this->fCache = *this->fZone; }
};

} // end namespace

#endif

class GUI;
namespace oscfaust
{

class OSCIO;
class RootNode;
typedef class SMARTP<RootNode> SRootNode;
class MessageDriven;
typedef class SMARTP<MessageDriven>	SMessageDriven;

//--------------------------------------------------------------------------
/*!
	\brief a factory to build a OSC UI hierarchy
	
	Actually, makes use of a stack to build the UI hierarchy.
	It includes a pointer to a OSCIO controler, but just to give it to the root node.
*/
class FaustFactory
{
    std::stack<SMessageDriven>  fNodes;		///< maintains the current hierarchy level
    SRootNode  fRoot;   ///< keep track of the root node
    OSCIO*     fIO;     ///< hack to support audio IO via OSC, actually the field is given to the root node
    GUI*       fGUI;    ///< a GUI pointer to support updateAllGuis(), required for bi-directionnal OSC
    JSONUI*    fJSON;
    
    private:
        std::string addressFirst(const std::string& address) const;
        std::string addressTail(const std::string& address) const;
        
    public:
        FaustFactory(GUI* ui, JSONUI* json, OSCIO * io = NULL);
        virtual ~FaustFactory();
        
        template <typename C> void addnode(const char* label, C* zone, C init, C min, C max, bool initZone, bool input);
        template <typename C> void addAlias(const std::string& fullpath, C* zone, C imin, C imax, C init, C min, C max, const char* label);
        
        void addAlias(const char* alias, const char* address, float imin, float imax, float omin, float omax);
        void addAlias(const char* alias, const char* address, double imin, double imax, double omin, double omax);
        void opengroup(const char* label);
        void closegroup();
        
        SRootNode root() const; 
};

/**
 * Add a node to the OSC UI tree in the current group at the top of the stack 
 */
template <typename C> void FaustFactory::addnode(const char* label, C* zone, C init, C min, C max, bool initZone, bool input) 
{
	SMessageDriven top;
	if (fNodes.size()) top = fNodes.top();
	if (top) {
		std::string prefix = top->getOSCAddress();
		top->add(FaustNode<C>::create(root(), label, zone, init, min, max, prefix.c_str(), fGUI, initZone, input));
	}
}

/**
 * Add an alias (actually stored and handled at root node level
 */
template <typename C> void FaustFactory::addAlias(const std::string& fullpath, C* zone, C imin, C imax, C init, C min, C max, const char* label)
{
	std::istringstream 	ss(fullpath);
	std::string 		realpath; 
 
	ss >> realpath >> imin >> imax;
	SMessageDriven top = fNodes.top();
	if (top) {
		std::string target = top->getOSCAddress() + "/" + label;
		addAlias(realpath.c_str(), target.c_str(), C(imin), C(imax), C(min), C(max));
	}
}

} // end namespoace

#endif

class GUI;

typedef void (*ErrorCallback)(void*);  

namespace oscfaust
{

class OSCIO;
class OSCSetup;
class OSCRegexp;
    
//--------------------------------------------------------------------------
/*!
	\brief the main Faust OSC Lib API
	
	The OSCControler is essentially a glue between the memory representation (in charge of the FaustFactory),
	and the network services (in charge of OSCSetup).
*/
class OSCControler
{
	int fUDPPort,   fUDPOut, fUPDErr;	// the udp ports numbers
	std::string     fDestAddress;		// the osc messages destination address, used at initialization only
										// to collect the address from the command line
	std::string     fBindAddress;		// when non empty, the address used to bind the socket for listening
	OSCSetup*		fOsc;				// the network manager (handles the udp sockets)
	OSCIO*			fIO;				// hack for OSC IO support (actually only relayed to the factory)
	FaustFactory*	fFactory;			// a factory to build the memory representation

    bool            fInit;
    
	public:
		/*
			base udp port is chosen in an unassigned range from IANA PORT NUMBERS (last updated 2011-01-24)
			see at http://www.iana.org/assignments/port-numbers
			5507-5552  Unassigned
		*/
		enum { kUDPBasePort = 5510 };
            
        OSCControler(int argc, char* argv[], GUI* ui, JSONUI* json, OSCIO* io = NULL, ErrorCallback errCallback = NULL, void* arg = NULL, bool init = true);

        virtual ~OSCControler();
	
		//--------------------------------------------------------------------------
		// addnode, opengroup and closegroup are simply relayed to the factory
		//--------------------------------------------------------------------------
		// Add a node in the current group (top of the group stack)
		template <typename T> void addnode(const char* label, T* zone, T init, T min, T max, bool input = true)
							{ fFactory->addnode(label, zone, init, min, max, fInit, input); }
		
		//--------------------------------------------------------------------------
		// This method is used for alias messages. The arguments imin and imax allow
		// to map incomming values from the alias input range to the actual range 
		template <typename T> void addAlias(const std::string& fullpath, T* zone, T imin, T imax, T init, T min, T max, const char* label)
							{ fFactory->addAlias(fullpath, zone, imin, imax, init, min, max, label); }

		void opengroup(const char* label)		{ fFactory->opengroup(label); }
		void closegroup()						{ fFactory->closegroup(); }
	   
		//--------------------------------------------------------------------------
		void run();				// starts the network services
		void endBundle();		// when bundle mode is on, close and send the current bundle (if any)
		void stop();			// stop the network services
		std::string getInfos() const; // gives information about the current environment (version, port numbers,...)

		int	getUDPPort() const			{ return fUDPPort; }
		int	getUDPOut()	const			{ return fUDPOut; }
		int	getUDPErr()	const			{ return fUPDErr; }
		const char*	getDestAddress() const { return fDestAddress.c_str(); }
		const char*	getRootName() const;	// probably useless, introduced for UI extension experiments
    
        void setUDPPort(int port) { fUDPPort = port; }
        void setUDPOut(int port) { fUDPOut = port; }
        void setUDPErr(int port) { fUPDErr = port; }
        void setDestAddress(const char* address) { fDestAddress = address; }

        // By default, an osc interface emits all parameters. You can filter specific params dynamically.
        static std::vector<OSCRegexp*>     fFilteredPaths; // filtered paths will not be emitted
        static void addFilteredPath(std::string path);
        static bool isPathFiltered(std::string path);
        static void resetFilteredPaths();
    
		static float version();				// the Faust OSC library version number
		static const char* versionstr();	// the Faust OSC library version number as a string
		static int gXmit;                   // a static variable to control the transmission of values
                                            // i.e. the use of the interface as a controler
		static int gBundle;                 // a static variable to control the osc bundle mode
};

#define kNoXmit     0
#define kAll        1
#define kAlias      2

}

#endif

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

/******************************************************************************
 *******************************************************************************
 
 OSC (Open Sound Control) USER INTERFACE
 
 *******************************************************************************
 *******************************************************************************/
/*
 
 Note about the OSC addresses and the Faust UI names:
 ----------------------------------------------------
 There are potential conflicts between the Faust UI objects naming scheme and
 the OSC address space. An OSC symbolic names is an ASCII string consisting of
 printable characters other than the following:
	space
 #	number sign
 *	asterisk
 ,	comma
 /	forward
 ?	question mark
 [	open bracket
 ]	close bracket
 {	open curly brace
 }	close curly brace
 
 a simple solution to address the problem consists in replacing
 space or tabulation with '_' (underscore)
 all the other osc excluded characters with '-' (hyphen)
 
 This solution is implemented in the proposed OSC UI;
 */

class OSCUI : public GUI
{
    
    private:
        
        oscfaust::OSCControler*	fCtrl;
        std::vector<const char*> fAlias;
        JSONUI fJSON;
        
        const char* tr(const char* label) const
        {
            static char buffer[1024];
            char * ptr = buffer; int n=1;
            while (*label && (n++ < 1024)) {
                switch (*label) {
                    case ' ': case '	':
                        *ptr++ = '_';
                        break;
                    case '#': case '*': case ',': case '/': case '?':
                    case '[': case ']': case '{': case '}': case '(': case ')':
                        *ptr++ = '_';
                        break;
                    default:
                        *ptr++ = *label;
                }
                label++;
            }
            *ptr = 0;
            return buffer;
        }
        
        // add all accumulated alias
        void addalias(FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, const char* label)
        {
            for (unsigned int i = 0; i < fAlias.size(); i++) {
                fCtrl->addAlias(fAlias[i], zone, FAUSTFLOAT(0), FAUSTFLOAT(1), init, min, max, label);
            }
            fAlias.clear();
        }
        
    public:
        
        OSCUI(const char* /*applicationname*/, int argc, char* argv[], oscfaust::OSCIO* io = NULL, ErrorCallback errCallback = NULL, void* arg = NULL, bool init = true) : GUI()
        {
            fCtrl = new oscfaust::OSCControler(argc, argv, this, &fJSON, io, errCallback, arg, init);
            //		fCtrl->opengroup(applicationname);
        }
        
        virtual ~OSCUI() { delete fCtrl; }
        
        // -- widget's layouts
        
        virtual void openTabBox(const char* label)          { fCtrl->opengroup(tr(label)); fJSON.openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fCtrl->opengroup(tr(label)); fJSON.openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fCtrl->opengroup(tr(label)); fJSON.openVerticalBox(label); }
        virtual void closeBox()                             { fCtrl->closegroup(); fJSON.closeBox(); }
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            const char* l = tr(label);
            addalias(zone, 0, 0, 1, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), FAUSTFLOAT(0), FAUSTFLOAT(1));
            fJSON.addButton(label, zone);
        }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            const char* l = tr(label);
            addalias(zone, 0, 0, 1, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), FAUSTFLOAT(0), FAUSTFLOAT(1));
            fJSON.addCheckButton(label, zone);
        }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addVerticalSlider(label, zone, init, min, max, step);
        }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addHorizontalSlider(label, zone, init, min, max, step);
        }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            const char* l = tr(label);
            addalias(zone, init, min, max, l);
            fCtrl->addnode(l, zone, init, min, max);
            fJSON.addNumEntry(label, zone, init, min, max, step);
        }
        
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            const char* l = tr(label);
            addalias(zone, 0, min, max, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), min, max, false);
            fJSON.addHorizontalBargraph(label, zone, min, max);
        }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            const char* l = tr(label);
            addalias(zone, 0, min, max, l);
            fCtrl->addnode(l, zone, FAUSTFLOAT(0), min, max, false);
            fJSON.addVerticalBargraph(label, zone, min, max);
        }
            
        // -- metadata declarations
        
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* alias)
        {
            if (strcasecmp(key, "OSC") == 0) fAlias.push_back(alias);
            fJSON.declare(zone, key, alias);
        }
        
        virtual void show() {}
        
        bool run()
        {
            fCtrl->run();
            return true;
        }
        
        void stop()			{ fCtrl->stop(); }
        void endBundle() 	{ fCtrl->endBundle(); }
        
        std::string getInfos()          { return fCtrl->getInfos(); }
        
        const char* getRootName()		{ return fCtrl->getRootName(); }
        int getUDPPort()                { return fCtrl->getUDPPort(); }
        int getUDPOut()                 { return fCtrl->getUDPOut(); }
        int getUDPErr()                 { return fCtrl->getUDPErr(); }
        const char* getDestAddress()    { return fCtrl->getDestAddress(); }
        
        void setUDPPort(int port)       { fCtrl->setUDPPort(port); }
        void setUDPOut(int port)        { fCtrl->setUDPOut(port); }
        void setUDPErr(int port)        { fCtrl->setUDPErr(port); }
        void setDestAddress(const char* address)    { return fCtrl->setDestAddress(address); }
    
};

#endif // __OSCUI__
/**************************  END  OSCUI.h **************************/

#ifdef POLY2
#include "effect.h"
#endif

#if SOUNDFILE
/************************** BEGIN SoundUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __SoundUI_H__
#define __SoundUI_H__

#include <map>
#include <vector>
#include <string>
#include <iostream>


#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif

// Always included otherwise -i mode later on will not always include it (with the conditional includes)
/************************** BEGIN Soundfile.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __Soundfile__
#define __Soundfile__

#include <iostream>
#include <string.h>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#define BUFFER_SIZE 16384
#define SAMPLE_RATE 44100
#define MAX_CHAN 64
#define MAX_SOUNDFILE_PARTS 256

#ifdef _MSC_VER
#define PRE_PACKED_STRUCTURE __pragma(pack(push, 1))
#define POST_PACKED_STRUCTURE \
    ;                         \
    __pragma(pack(pop))
#else
#define PRE_PACKED_STRUCTURE
#define POST_PACKED_STRUCTURE __attribute__((__packed__))
#endif

/*
 The soundfile structure to be used by the DSP code. Soundfile has a MAX_SOUNDFILE_PARTS parts 
 (even a single soundfile or an empty soundfile). 
 fLength, fOffset and fSR fields are filled accordingly by repeating
 the actual parts if needed.
 
 It has to be 'packed' to that the LLVM backend can correctly access it.

 Index computation:
    - p is the current part number [0..MAX_SOUNDFILE_PARTS-1] (must be proved by the type system)
    - i is the current position in the part. It will be constrained between [0..length]
    - idx(p,i) = fOffset[p] + max(0, min(i, fLength[p]));
*/

PRE_PACKED_STRUCTURE
struct Soundfile {
    FAUSTFLOAT** fBuffers;
    int* fLength;   // length of each part
    int* fSR;       // sample rate of each part
    int* fOffset;   // offset of each part in the global buffer
    int fChannels;  // max number of channels of all concatenated files

    Soundfile()
    {
        fBuffers  = nullptr;
        fChannels = -1;
        fLength   = new int[MAX_SOUNDFILE_PARTS];
        fSR       = new int[MAX_SOUNDFILE_PARTS];
        fOffset   = new int[MAX_SOUNDFILE_PARTS];
    }

    ~Soundfile()
    {
        // Free the real channels only
        for (int chan = 0; chan < fChannels; chan++) {
            delete fBuffers[chan];
        }
        delete[] fBuffers;
        delete[] fLength;
        delete[] fSR;
        delete[] fOffset;
    }

} POST_PACKED_STRUCTURE;

/*
 The generic soundfile reader.
 */

class SoundfileReader {
    
   protected:
    
    int fDriverSR;
    
    void emptyFile(Soundfile* soundfile, int part, int& offset)
    {
        soundfile->fLength[part] = BUFFER_SIZE;
        soundfile->fSR[part] = SAMPLE_RATE;
        soundfile->fOffset[part] = offset;
        // Update offset
        offset += soundfile->fLength[part];
    }

    Soundfile* createSoundfile(int cur_chan, int length, int max_chan)
    {
        Soundfile* soundfile = new Soundfile();
        soundfile->fBuffers = new FAUSTFLOAT*[max_chan];
        
        for (int chan = 0; chan < cur_chan; chan++) {
            soundfile->fBuffers[chan] = new FAUSTFLOAT[length];
            memset(soundfile->fBuffers[chan], 0, sizeof(FAUSTFLOAT) * length);
        }
        
        soundfile->fChannels = cur_chan;
        return soundfile;
    }

    void getBuffersOffset(Soundfile* soundfile, FAUSTFLOAT** buffers, int offset)
    {
        for (int chan = 0; chan < soundfile->fChannels; chan++) {
            buffers[chan] = &soundfile->fBuffers[chan][offset];
        }
    }
    
    // Check if a soundfile exists and return its real path_name
    std::string checkFile(const std::vector<std::string>& sound_directories, const std::string& file_name)
    {
        if (checkFile(file_name)) {
            return file_name;
        } else {
            for (size_t i = 0; i < sound_directories.size(); i++) {
                std::string path_name = sound_directories[i] + "/" + file_name;
                if (checkFile(path_name)) { return path_name; }
            }
            return "";
        }
    }
    
    bool isResampling(int sample_rate) { return (fDriverSR > 0 && fDriverSR != sample_rate); }
 
    // To be implemented by subclasses

    /**
     * Check the availability of a sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     *
     * @return true if the sound resource is available, false otherwise.
     */
    virtual bool checkFile(const std::string& path_name) = 0;
    
    /**
     * Check the availability of a sound resource.
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     *
     * @return true if the sound resource is available, false otherwise.
     */

    virtual bool checkFile(unsigned char* buffer, size_t length) { return true; }

    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(const std::string& path_name, int& channels, int& length) = 0;
    
    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(unsigned char* buffer, size_t size, int& channels, int& length) {}

    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan) = 0;
    
    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param buffer - the sound buffer
     * @param buffer - the sound buffer length
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, unsigned char* buffer, size_t length, int part, int& offset, int max_chan) {}

  public:
    
    virtual ~SoundfileReader() {}
    
    void setSampleRate(int sample_rate) { fDriverSR = sample_rate; }
   
    Soundfile* createSoundfile(const std::vector<std::string>& path_name_list, int max_chan)
    {
        try {
            int cur_chan = 1; // At least one buffer
            int total_length = 0;
            
            // Compute total length and channels max of all files
            for (int i = 0; i < int(path_name_list.size()); i++) {
                int chan, length;
                if (path_name_list[i] == "__empty_sound__") {
                    length = BUFFER_SIZE;
                    chan = 1;
                } else {
                    getParamsFile(path_name_list[i], chan, length);
                }
                cur_chan = std::max<int>(cur_chan, chan);
                total_length += length;
            }
           
            // Complete with empty parts
            total_length += (MAX_SOUNDFILE_PARTS - path_name_list.size()) * BUFFER_SIZE;
            
            // Create the soundfile
            Soundfile* soundfile = createSoundfile(cur_chan, total_length, max_chan);
            
            // Init offset
            int offset = 0;
            
            // Read all files
            for (int i = 0; i < int(path_name_list.size()); i++) {
                if (path_name_list[i] == "__empty_sound__") {
                    emptyFile(soundfile, i, offset);
                } else {
                    readFile(soundfile, path_name_list[i], i, offset, max_chan);
                }
            }
            
            // Complete with empty parts
            for (int i = int(path_name_list.size()); i < MAX_SOUNDFILE_PARTS; i++) {
                emptyFile(soundfile, i, offset);
            }
            
            // Share the same buffers for all other channels so that we have max_chan channels available
            for (int chan = cur_chan; chan < max_chan; chan++) {
                soundfile->fBuffers[chan] = soundfile->fBuffers[chan % cur_chan];
            }
            
            return soundfile;
            
        } catch (...) {
            return nullptr;
        }
    }

    // Check if all soundfiles exist and return their real path_name
    std::vector<std::string> checkFiles(const std::vector<std::string>& sound_directories,
                                        const std::vector<std::string>& file_name_list)
    {
        std::vector<std::string> path_name_list;
        for (size_t i = 0; i < file_name_list.size(); i++) {
            std::string path_name = checkFile(sound_directories, file_name_list[i]);
            // If 'path_name' is not found, it is replaced by an empty sound (= silence)
            path_name_list.push_back((path_name == "") ? "__empty_sound__" : path_name);
        }
        return path_name_list;
    }

};

#endif
/**************************  END  Soundfile.h **************************/

#if defined(JUCE_32BIT) || defined(JUCE_64BIT)
/************************** BEGIN JuceReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JuceReader__
#define __JuceReader__

#include <assert.h>

#include "../JuceLibraryCode/JuceHeader.h"


struct JuceReader : public SoundfileReader {
    
    AudioFormatManager fFormatManager;
    
    JuceReader() { fFormatManager.registerBasicFormats(); }
    virtual ~JuceReader()
    {}
    
    bool checkFile(const std::string& path_name)
    {
        File file(path_name);
        if (file.existsAsFile()) {
            return true;
        } else {
            std::cerr << "ERROR : cannot open '" << path_name << "'" << std::endl;
            return false;
        }
    }
    
    void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        std::unique_ptr<AudioFormatReader> formatReader (fFormatManager.createReaderFor (File (path_name)));
        channels = int(formatReader->numChannels);
        length = int(formatReader->lengthInSamples);
    }
    
    void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        std::unique_ptr<AudioFormatReader> formatReader (fFormatManager.createReaderFor (File (path_name)));
        
        soundfile->fLength[part] = int(formatReader->lengthInSamples);
        soundfile->fSR[part] = int(formatReader->sampleRate);
        soundfile->fOffset[part] = offset;
        
        FAUSTFLOAT** buffers = static_cast<FAUSTFLOAT**>(alloca(soundfile->fChannels * sizeof(FAUSTFLOAT*)));
                                                             
        getBuffersOffset(soundfile, buffers, offset);
        
        if (formatReader->read(reinterpret_cast<int *const *>(buffers), int(formatReader->numChannels), 0, int(formatReader->lengthInSamples), false)) {
            
            // Possibly concert samples
            if (!formatReader->usesFloatingPointData) {
                for (int chan = 0; chan < int(formatReader->numChannels); ++chan) {
                    FAUSTFLOAT* buffer = &soundfile->fBuffers[chan][soundfile->fOffset[part]];
                    FloatVectorOperations::convertFixedToFloat(buffer, reinterpret_cast<const int*>(buffer), 1.0f/0x7fffffff, int(formatReader->lengthInSamples));
                }
            }
            
        } else {
            std::cerr << "Error reading the file : " << path_name << std::endl;
        }
            
        // Update offset
        offset += soundfile->fLength[part];
    }
    
};

#endif
/**************************  END  JuceReader.h **************************/
JuceReader gReader;
#elif defined(ESP32)
/************************** BEGIN WaveReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __WaveReader__
#define __WaveReader__

#include <string.h>
#include <assert.h>
#include <iostream>


// WAVE file description
typedef struct {
    
    // The canonical WAVE format starts with the RIFF header
    
    /**
     Variable: chunk_id
     Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).
     **/
    int chunk_id;
    
    /**
     Variable: chunk_size
     36 + SubChunk2Size, or more precisely: 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
     This is the size of the rest of the chunk following this number.
     This is the size of the entire file in bytes minus 8 bytes for the
     two fields not included in this count: ChunkID and ChunkSize.
     **/
    int chunk_size;
    
    /**
     Variable: format
     Contains the letters "WAVE" (0x57415645 big-endian form).
     **/
    int format;
    
    // The "WAVE" format consists of two subchunks: "fmt " and "data":
    // The "fmt " subchunk describes the sound data's format:
    
    /**
     Variable: subchunk_1_id
     Contains the letters "fmt " (0x666d7420 big-endian form).
     **/
    int subchunk_1_id;
    
    /**
     Variable: subchunk_1_size
     16 for PCM. This is the size of the rest of the Subchunk which follows this number.
     **/
    int subchunk_1_size;
    
    /**
     Variable: audio_format
     PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression.
     **/
    short audio_format;
    
    /**
     Variable: num_channels
     Mono = 1, Stereo = 2, etc.
     **/
    short num_channels;
    
    /**
     Variable: sample_rate
     8000, 44100, etc.
     **/
    int sample_rate;
    
    /**
     Variable: byte_rate
     == SampleRate * NumChannels * BitsPerSample/8
     **/
    int byte_rate;
    
    /**
     Variable: block_align
     == NumChannels * BitsPerSample/8
     The number of bytes for one sample including all channels. I wonder what happens
     when this number isn't an integer?
     **/
    short block_align;
    
    /**
     Variable: bits_per_sample
     8 bits = 8, 16 bits = 16, etc.
     **/
    short bits_per_sample;
    
    /**
     Here should come some extra parameters which i will avoid.
     **/
    
    // The "data" subchunk contains the size of the data and the actual sound:
    
    /**
     Variable: subchunk_2_id
     Contains the letters "data" (0x64617461 big-endian form).
     **/
    int subchunk_2_id;
    
    /**
     Variable: subchunk_2_size
     == NumSamples * NumChannels * BitsPerSample/8
     This is the number of bytes in the data. You can also think of this as the size
     of the read of the subchunk following this number.
     **/
    int subchunk_2_size;
    
    /**
     Variable: data
     The actual sound data.
     **/
    char* data;
    
} wave_t;

// Base reader
struct Reader {
    
    wave_t* fWave;

    inline int is_big_endian()
    {
        int a = 1;
        return !((char*)&a)[0];
    }
    
    inline int convert_to_int(char* buffer, int len)
    {
        int a = 0;
        if (!is_big_endian()) {
            for(int i = 0; i < len; i++) {
                ((char*)&a)[i] = buffer[i];
            }
        } else {
            for(int i = 0; i < len; i++) {
                ((char*)&a)[3-i] = buffer[i];
            }
        }
        return a;
    }
    
    Reader()
    {
        fWave = (wave_t*)calloc(1, sizeof(wave_t));
    }

    virtual ~Reader()
    {
        free(fWave->data);
        free(fWave);
    }

    bool load_wave_header()
    {
        char buffer[4];
        
        read(buffer, 4);
        if (strncmp(buffer, "RIFF", 4) != 0) {
            std::cerr << "This is not valid WAV file!\n";
            return false;
        }
        fWave->chunk_id = convert_to_int(buffer, 4);
        
        read(buffer, 4);
        fWave->chunk_size = convert_to_int(buffer, 4);
        
        read(buffer, 4);
        fWave->format = convert_to_int(buffer, 4);
        
        read(buffer, 4);
        fWave->subchunk_1_id = convert_to_int(buffer, 4);
        
        read(buffer, 4);
        fWave->subchunk_1_size = convert_to_int(buffer, 4);
        
        read(buffer, 2);
        fWave->audio_format = convert_to_int(buffer, 2);
        
        read(buffer, 2);
        fWave->num_channels = convert_to_int(buffer, 2);
        
        read(buffer, 4);
        fWave->sample_rate = convert_to_int(buffer, 4);
        
        read(buffer, 4);
        fWave->byte_rate = convert_to_int(buffer, 4);
        
        read(buffer, 2);
        fWave->block_align = convert_to_int(buffer, 2);
        
        read(buffer, 2);
        fWave->bits_per_sample = convert_to_int(buffer, 2);
        
        read(buffer, 4);
        if (strncmp(buffer, "data", 4) != 0) {
            read(buffer, 4);
            int _extra_size = convert_to_int(buffer, 4);
            char _extra_data[_extra_size];
            read(_extra_data, _extra_size);
            read(buffer, 4);
            fWave->subchunk_2_id = convert_to_int(buffer, 4);
        } else {
            fWave->subchunk_2_id = convert_to_int(buffer, 4);
        }
        
        read(buffer, 4);
        fWave->subchunk_2_size = convert_to_int(buffer, 4);
        return true;
    }
    
    void load_wave()
    {
        // Read sound data
        fWave->data = (char*)malloc(fWave->subchunk_2_size);
        read(fWave->data, fWave->subchunk_2_size);
    }

    virtual void read(char* buffer, unsigned int size) = 0;
   
};

struct FileReader : public Reader {
    
    FILE* fFile;
    
    FileReader(const std::string& file_path)
    {
        fFile = fopen(file_path.c_str(), "rb");
        if (!fFile) {
            std::cerr << "FileReader : cannot open file!\n";
            throw -1;
        }
        if (!load_wave_header()) {
            std::cerr << "FileReader : not a WAV file!\n";
            throw -1;
        }
    }
    
    virtual ~FileReader()
    {
        fclose(fFile);
    }
    
    void read(char* buffer, unsigned int size)
    {
        fread(buffer, 1, size, fFile);
    }
    
};

extern const uint8_t file_start[] asm("_binary_FILE_start");
extern const uint8_t file_end[]   asm("_binary_FILE_end");

struct MemoryReader : public Reader {
    
    int fPos;
    const uint8_t* fStart;
    const uint8_t* fEnd;
    
    MemoryReader(const uint8_t* start, const uint8_t* end):fPos(0)
    {
        fStart = start;
        fEnd = end;
        if (!load_wave_header()) {
            std::cerr << "MemoryReader : not a WAV file!\n";
            throw -1;
        }
    }
    
    virtual ~MemoryReader()
    {}
    
    void read(char* buffer, unsigned int size)
    {
        memcpy(buffer, fStart + fPos, size);
        fPos += size;
    }
    
};


// Using a FileReader to implement SoundfileReader

struct WaveReader : public SoundfileReader {
    
    WaveReader() {}
    
    bool checkFile(const std::string& path_name)
    {
        try {
            Reader* reader = new FileReader(path_name);
            delete reader;
            return true;
        } catch(...)  {
            return false;
        }
    }
    
    void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        Reader* reader = new FileReader(path_name);
        assert(reader);
        channels = reader->fWave->num_channels;
        length = (reader->fWave->subchunk_2_size * 8) / (reader->fWave->num_channels * reader->fWave->bits_per_sample);
        delete reader;
    }
    
    void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        Reader* reader = new FileReader(path_name);
        assert(reader);
        reader->load_wave();
        
        soundfile->fLength[part] = (reader->fWave->subchunk_2_size * 8) / (reader->fWave->num_channels * reader->fWave->bits_per_sample);
        soundfile->fSR[part] = reader->fWave->sample_rate;
        soundfile->fOffset[part] = offset;
        
        // Audio frames have to be written for each chan
        if (reader->fWave->bits_per_sample == 16) {
            float factor = 1.f/32767.f;
            for (int sample = 0; sample < soundfile->fLength[part]; sample++) {
                short* frame = (short*)&reader->fWave->data[reader->fWave->block_align * sample];
                for (int chan = 0; chan < reader->fWave->num_channels; chan++) {
                    soundfile->fBuffers[chan][offset + sample] = frame[chan] * factor;
                }
            }
        } else if (reader->fWave->bits_per_sample == 32) {
            std::cerr << "readFile : not implemented \n";
        }
        
        // Update offset
        offset += soundfile->fLength[part];
        delete reader;
    }
};

#endif
/**************************  END  WaveReader.h **************************/
WaveReader gReader;
#elif defined(MEMORY_READER)
/************************** BEGIN MemoryReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __MemoryReader__
#define __MemoryReader__


/*
 A 'MemoryReader' object can be used to prepare a set of sound resources in memory, to be used by SoundUI::addSoundfile.
 
 A Soundfile* object will have to be filled with a list of sound resources: the fLength, fOffset, fSampleRate and fBuffers fields 
 have to be completed with the appropriate values, and will be accessed in the DSP object while running.
 *
 */

// To adapt for a real case use

#define SOUND_CHAN      2
#define SOUND_LENGTH    4096
#define SOUND_SR        40100

struct MemoryReader : public SoundfileReader {
    
    MemoryReader()
    {}
    
    /**
     * Check the availability of a sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     *
     * @return true if the sound resource is available, false otherwise.
     */
    virtual bool checkFile(const std::string& path_name) { return true; }
    
    /**
     * Get the channels and length values of the given sound resource.
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param channels - the channels value to be filled with the sound resource number of channels
     * @param length - the length value to be filled with the sound resource length in frames
     *
     */
    virtual void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        channels = SOUND_CHAN;
        length = SOUND_LENGTH;
    }
    
    /**
     * Read one sound resource and fill the 'soundfile' structure accordingly
     *
     * @param path_name - the name of the file, or sound resource identified this way
     * @param part - the part number to be filled in the soundfile
     * @param offset - the offset value to be incremented with the actual sound resource length in frames
     * @param max_chan - the maximum number of mono channels to fill
     *
     */
    virtual void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        soundfile->fLength[part] = SOUND_LENGTH;
        soundfile->fSR[part] = SOUND_SR;
        soundfile->fOffset[part] = offset;
        
        // Audio frames have to be written for each chan
        for (int sample = 0; sample < SOUND_LENGTH; sample++) {
            for (int chan = 0; chan < SOUND_CHAN; chan++) {
                soundfile->fBuffers[chan][offset + sample] = 0.f;
            }
        }
        
        // Update offset
        offset += SOUND_LENGTH;
    }
    
};

#endif
/**************************  END  MemoryReader.h **************************/
MemoryReader gReader;
#else
/************************** BEGIN LibsndfileReader.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __LibsndfileReader__
#define __LibsndfileReader__

#ifdef SAMPLERATE
#include <samplerate.h>
#endif
#include <sndfile.h>
#include <string.h>
#include <assert.h>
#include <iostream>


struct VFLibsndfile {
    
    #define SIGNED_SIZEOF(x) ((int)sizeof(x))
    
    unsigned char* fBuffer;
    size_t fLength;
    size_t fOffset;
    SF_VIRTUAL_IO fVIO;
    
    VFLibsndfile(unsigned char* buffer, size_t length):fBuffer(buffer), fLength(length), fOffset(0)
    {
        fVIO.get_filelen = vfget_filelen;
        fVIO.seek = vfseek;
        fVIO.read = vfread;
        fVIO.write = vfwrite;
        fVIO.tell = vftell;
    }
    
    static sf_count_t vfget_filelen(void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        return vf->fLength;
    }
  
    static sf_count_t vfseek(sf_count_t offset, int whence, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        switch (whence) {
            case SEEK_SET:
                vf->fOffset = offset;
                break;
                
            case SEEK_CUR:
                vf->fOffset = vf->fOffset + offset;
                break;
                
            case SEEK_END:
                vf->fOffset = vf->fLength + offset;
                break;
                
            default:
                break;
        };
        
        return vf->fOffset;
    }
    
    static sf_count_t vfread(void* ptr, sf_count_t count, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        
        /*
         **	This will break badly for files over 2Gig in length, but
         **	is sufficient for testing.
         */
        if (vf->fOffset + count > vf->fLength) {
            count = vf->fLength - vf->fOffset;
        }
        
        memcpy(ptr, vf->fBuffer + vf->fOffset, count);
        vf->fOffset += count;
        
        return count;
    }
    
    static sf_count_t vfwrite(const void* ptr, sf_count_t count, void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        
        /*
         **	This will break badly for files over 2Gig in length, but
         **	is sufficient for testing.
         */
        if (vf->fOffset >= SIGNED_SIZEOF(vf->fBuffer)) {
            return 0;
        }
        
        if (vf->fOffset + count > SIGNED_SIZEOF(vf->fBuffer)) {
            count = sizeof (vf->fBuffer) - vf->fOffset;
        }
        
        memcpy(vf->fBuffer + vf->fOffset, ptr, (size_t)count);
        vf->fOffset += count;
        
        if (vf->fOffset > vf->fLength) {
            vf->fLength = vf->fOffset;
        }
        
        return count;
    }
    
    static sf_count_t vftell(void* user_data)
    {
        VFLibsndfile* vf = static_cast<VFLibsndfile*>(user_data);
        return vf->fOffset;
    }
 
};

struct LibsndfileReader : public SoundfileReader {
	
    LibsndfileReader() {}
	
    typedef sf_count_t (* sample_read)(SNDFILE* sndfile, FAUSTFLOAT* ptr, sf_count_t frames);
	
    // Check file
    bool checkFile(const std::string& path_name)
    {
        SF_INFO snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        return checkFileAux(snd_file, path_name);
    }
    
    bool checkFile(unsigned char* buffer, size_t length)
    {
        SF_INFO snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, length);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        return checkFileAux(snd_file, "virtual file");
    }
    
    bool checkFileAux(SNDFILE* snd_file, const std::string& path_name)
    {
        if (snd_file) {
            sf_close(snd_file);
            return true;
        } else {
            std::cerr << "ERROR : cannot open '" << path_name << "' (" << sf_strerror(NULL) << ")" << std::endl;
            return false;
        }
    }

    // Open the file and returns its length and channels
    void getParamsFile(const std::string& path_name, int& channels, int& length)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        getParamsFileAux(snd_file, snd_info, channels, length);
    }
    
    void getParamsFile(unsigned char* buffer, size_t size, int& channels, int& length)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, size);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        getParamsFileAux(snd_file, snd_info, channels, length);
    }
    
    void getParamsFileAux(SNDFILE* snd_file, const SF_INFO& snd_info, int& channels, int& length)
    {
        assert(snd_file);
        channels = int(snd_info.channels);
    #ifdef SAMPLERATE
        length = (isResampling(snd_info.samplerate)) ? ((double(snd_info.frames) * double(fDriverSR) / double(snd_info.samplerate)) + BUFFER_SIZE) : int(snd_info.frames);
    #else
        length = int(snd_info.frames);
    #endif
        sf_close(snd_file);
    }
    
    // Read the file
    void copyToOut(Soundfile* soundfile, int size, int channels, int max_channels, int offset, FAUSTFLOAT* buffer)
    {
        for (int sample = 0; sample < size; sample++) {
            for (int chan = 0; chan < channels; chan++) {
                soundfile->fBuffers[chan][offset + sample] = buffer[sample * max_channels + chan];
            }
        }
    }
    
    void readFile(Soundfile* soundfile, const std::string& path_name, int part, int& offset, int max_chan)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        SNDFILE* snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
        readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
    }
    
    void readFile(Soundfile* soundfile, unsigned char* buffer, size_t length, int part, int& offset, int max_chan)
    {
        SF_INFO	snd_info;
        snd_info.format = 0;
        VFLibsndfile vio(buffer, length);
        SNDFILE* snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
        readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
    }
	
    // Will be called to fill all parts from 0 to MAX_SOUNDFILE_PARTS-1
    void readFileAux(Soundfile* soundfile, SNDFILE* snd_file, const SF_INFO& snd_info, int part, int& offset, int max_chan)
    {
        assert(snd_file);
        int channels = std::min<int>(max_chan, snd_info.channels);
    #ifdef SAMPLERATE
        if (isResampling(snd_info.samplerate)) {
            soundfile->fLength[part] = int(double(snd_info.frames) * double(fDriverSR) / double(snd_info.samplerate));
            soundfile->fSR[part] = fDriverSR;
        } else {
            soundfile->fLength[part] = int(snd_info.frames);
            soundfile->fSR[part] = snd_info.samplerate;
        }
    #else
        soundfile->fLength[part] = int(snd_info.frames);
        soundfile->fSR[part] = snd_info.samplerate;
    #endif
        soundfile->fOffset[part] = offset;
		
        // Read and fill snd_info.channels number of channels
        sf_count_t nbf;
        FAUSTFLOAT* buffer_in = (FAUSTFLOAT*)alloca(BUFFER_SIZE * sizeof(FAUSTFLOAT) * snd_info.channels);
        sample_read reader;
        
        if (sizeof(FAUSTFLOAT) == 4) {
            reader = reinterpret_cast<sample_read>(sf_readf_float);
        } else {
            reader = reinterpret_cast<sample_read>(sf_readf_double);
        }
        
    #ifdef SAMPLERATE
        // Resampling
        SRC_STATE* resampler = nullptr;
        FAUSTFLOAT* buffer_out = nullptr;
        if  (isResampling(snd_info.samplerate)) {
            int error;
            resampler = src_new(SRC_SINC_FASTEST, channels, &error);
            if (error != 0) {
                std::cerr << "ERROR : src_new " << src_strerror(error) << std::endl;
                throw -1;
            }
            buffer_out = (FAUSTFLOAT*)alloca(BUFFER_SIZE * sizeof(FAUSTFLOAT) * snd_info.channels);
        }
    #endif
        
        do {
            nbf = reader(snd_file, buffer_in, BUFFER_SIZE);
        #ifdef SAMPLERATE
            // Resampling
            if  (isResampling(snd_info.samplerate)) {
                int in_offset = 0;
                SRC_DATA src_data;
                src_data.src_ratio = double(fDriverSR)/double(snd_info.samplerate);
                do {
                    src_data.data_in = &buffer_in[in_offset * snd_info.channels];
                    src_data.data_out = buffer_out;
                    src_data.input_frames = nbf - in_offset;
                    src_data.output_frames = BUFFER_SIZE;
                    src_data.end_of_input = (nbf < BUFFER_SIZE);
                    int res = src_process(resampler, &src_data);
                    if (res != 0) {
                        std::cerr << "ERROR : src_process " << src_strerror(res) << std::endl;
                        throw -1;
                    }
                    copyToOut(soundfile, src_data.output_frames_gen, channels, snd_info.channels, offset, buffer_out);
                    in_offset += src_data.input_frames_used;
                    // Update offset
                    offset += src_data.output_frames_gen;
                } while (in_offset < nbf);
            } else {
                copyToOut(soundfile, nbf, channels, snd_info.channels, offset, buffer_in);
                // Update offset
                offset += nbf;
            }
        #else
            copyToOut(soundfile, nbf, channels, snd_info.channels, offset, buffer_in);
            // Update offset
            offset += nbf;
        #endif
        } while (nbf == BUFFER_SIZE);
		
        sf_close(snd_file);
    #ifdef SAMPLERATE
        if (resampler) src_delete(resampler);
    #endif
    }

};

#endif
/**************************  END  LibsndfileReader.h **************************/
LibsndfileReader gReader;
#endif

// To be used by DSP code if no SoundUI is used
std::vector<std::string> path_name_list;
Soundfile* defaultsound = gReader.createSoundfile(path_name_list, MAX_CHAN);

class SoundUI : public GenericUI
{
		
    private:
    
        std::vector<std::string> fSoundfileDir;             // The soundfile directories
        std::map<std::string, Soundfile*> fSoundfileMap;    // Map to share loaded soundfiles
        SoundfileReader* fSoundReader;

     public:
    
        /**
         * Create a soundfile loader which will typically use a concrete SoundfileReader like LibsndfileReader or JuceReader to load soundfiles.
         *
         * @param sound_directory - the base directory to look for files, which paths will be relative to this one
         * @param sample_rate - the audio driver SR which may be different from the file SR, to possibly resample files
         * @param reader - an alternative soundfile reader
         *
         * @return the soundfile loader.
         */
        SoundUI(const std::string& sound_directory = "", int sample_rate = -1, SoundfileReader* reader = nullptr)
        {
            fSoundfileDir.push_back(sound_directory);
            fSoundReader = (reader) ? reader : &gReader;
            fSoundReader->setSampleRate(sample_rate);
        }
    
        /**
         * Create a soundfile loader which will typically use a concrete SoundfileReader like LibsndfileReader or JuceReader to load soundfiles.
         *
         * @param sound_directories - a vector of base directories to look for files, which paths will be relative to these ones
         * @param sample_rate - the audio driver SR which may be different from the file SR, to possibly resample files
         * @param reader - an alternative soundfile reader
         *
         * @return the soundfile loader.
         */
        SoundUI(const std::vector<std::string>& sound_directories, int sample_rate = -1, SoundfileReader* reader = nullptr)
        :fSoundfileDir(sound_directories)
        {
            fSoundReader = (reader) ? reader : &gReader;
            fSoundReader->setSampleRate(sample_rate);
        }
    
        virtual ~SoundUI()
        {   
            // Delete all soundfiles
            std::map<std::string, Soundfile*>::iterator it;
            for (it = fSoundfileMap.begin(); it != fSoundfileMap.end(); it++) {
                delete (*it).second;
            }
        }

        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
        {
            const char* saved_url = url; // 'url' is consumed by parseMenuList2
            std::vector<std::string> file_name_list;
            
            bool menu = parseMenuList2(url, file_name_list, true);
            // If not a list, we have as single file
            if (!menu) { file_name_list.push_back(saved_url); }
            
            // Parse the possible list
            if (fSoundfileMap.find(saved_url) == fSoundfileMap.end()) {
                // Check all files and get their complete path
                std::vector<std::string> path_name_list = fSoundReader->checkFiles(fSoundfileDir, file_name_list);
                // Read them and create the Soundfile
                Soundfile* sound_file = fSoundReader->createSoundfile(path_name_list, MAX_CHAN);
                if (sound_file) {
                    fSoundfileMap[saved_url] = sound_file;
                } else {
                    // If failure, use 'defaultsound'
                    std::cerr << "addSoundfile : soundfile for " << saved_url << " cannot be created !" << std::endl;
                    *sf_zone = defaultsound;
                    return;
                }
            }
            
            // Get the soundfile
            *sf_zone = fSoundfileMap[saved_url];
        }
    
        /**
         * An OS dependant function to get the path of the running executable or plugin.
         * This will typically be used when creating a SoundUI soundfile loader, like new SoundUI(SoundUI::getBinaryPath());
         *
         * @return the running executable or plugin path.
         */
        static std::string getBinaryPath()
        {
            std::string bundle_path_str;
        #ifdef __APPLE__
            CFURLRef bundle_ref = CFBundleCopyBundleURL(CFBundleGetMainBundle());
            if (!bundle_ref) { std::cerr << "getBinaryPath CFBundleCopyBundleURL error\n"; return ""; }
      
            UInt8 bundle_path[1024];
            if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
                bundle_path_str = std::string((char*)bundle_path);
            } else {
                std::cerr << "getBinaryPath CFURLGetFileSystemRepresentation error\n";
            }
        #endif
        #ifdef ANDROID_DRIVER
            bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
        #endif
            return bundle_path_str;
        }
    
        /**
         * An OS dependant function to get the path of the running executable or plugin.
         * This will typically be used when creating a SoundUI soundfile loader, like new SoundUI(SoundUI::getBinaryPathFrom());
         *
         * @param path - entry point to start getting the path of the running executable or plugin.
         *
         * @return the running executable or plugin path.
         */
        static std::string getBinaryPathFrom(const std::string& path)
        {
            std::string bundle_path_str;
        #ifdef __APPLE__
            CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFStringCreateWithCString(kCFAllocatorDefault, path.c_str(), CFStringGetSystemEncoding()));
            if (!bundle) { std::cerr << "getBinaryPathFrom CFBundleGetBundleWithIdentifier error '" << path << "'" << std::endl; return ""; }
         
            CFURLRef bundle_ref = CFBundleCopyBundleURL(bundle);
            if (!bundle_ref) { std::cerr << "getBinaryPathFrom CFBundleCopyBundleURL error\n"; return ""; }
            
            UInt8 bundle_path[1024];
            if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
                bundle_path_str = std::string((char*)bundle_path);
            } else {
                std::cerr << "getBinaryPathFrom CFURLGetFileSystemRepresentation error\n";
            }
        #endif
        #ifdef ANDROID_DRIVER
            bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
        #endif
            return bundle_path_str;
        }
};

#endif
/**************************  END  SoundUI.h **************************/
#endif

// For FAUST_CLASS_NAME to be defined
#define FAUST_UIMACROS

// but we will ignore most of them
#define FAUST_ADDBUTTON(l,f)
#define FAUST_ADDCHECKBOX(l,f)
#define FAUST_ADDVERTICALSLIDER(l,f,i,a,b,s)
#define FAUST_ADDHORIZONTALSLIDER(l,f,i,a,b,s)
#define FAUST_ADDNUMENTRY(l,f,i,a,b,s)
#define FAUST_ADDVERTICALBARGRAPH(l,f,a,b)
#define FAUST_ADDHORIZONTALBARGRAPH(l,f,a,b)
#define FAUST_ADDSOUNDFILE(s,f)

using namespace std;

/******************************************************************************
*******************************************************************************

							       VECTOR INTRINSICS

*******************************************************************************
*******************************************************************************/


/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS ue_binaural_decoder
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class ue_binaural_decoder : public dsp {
	
 public:
	
	int fSampleRate;
	double fConst0;
	FAUSTFLOAT fVslider0;
	double fRec1[2];
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fVslider1;
	double fRec3[2];
	double fRec2[2];
	FAUSTFLOAT fVbargraph0;
	int IOTA;
	double fVec0[128];
	FAUSTFLOAT fCheckbox2;
	double fRec4[2];
	FAUSTFLOAT fVbargraph1;
	double fVec1[128];
	FAUSTFLOAT fCheckbox3;
	double fRec5[2];
	FAUSTFLOAT fVbargraph2;
	double fVec2[128];
	FAUSTFLOAT fCheckbox4;
	double fRec6[2];
	FAUSTFLOAT fVbargraph3;
	double fVec3[128];
	FAUSTFLOAT fCheckbox5;
	double fRec7[2];
	FAUSTFLOAT fVbargraph4;
	double fVec4[128];
	FAUSTFLOAT fCheckbox6;
	FAUSTFLOAT fCheckbox7;
	double fRec8[2];
	FAUSTFLOAT fVbargraph5;
	double fVec5[128];
	FAUSTFLOAT fCheckbox8;
	double fRec9[2];
	FAUSTFLOAT fVbargraph6;
	double fVec6[128];
	FAUSTFLOAT fCheckbox9;
	double fRec10[2];
	FAUSTFLOAT fVbargraph7;
	double fVec7[128];
	FAUSTFLOAT fCheckbox10;
	FAUSTFLOAT fCheckbox11;
	double fRec11[2];
	FAUSTFLOAT fVbargraph8;
	double fVec8[128];
	double fRec0[2];
	FAUSTFLOAT fHbargraph0;
	double fRec12[2];
	FAUSTFLOAT fHbargraph1;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Lecomte");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "0.1");
		m->declare("copyright", "(c) Pierre Lecomte 2015");
		m->declare("filename", "ue.binaural.decoder.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("gui.lib/author", "Pierre Lecomte");
		m->declare("gui.lib/copyright", "(c) Pierre Lecomte 2016");
		m->declare("gui.lib/license", "GPL");
		m->declare("gui.lib/name", "GUI Library");
		m->declare("gui.lib/version", "1.0");
		m->declare("license", "GPL)");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.3");
		m->declare("name", "Binaural decoder");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "0.1");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "0.0");
		m->declare("version", "1.0");
	}

	virtual int getNumInputs() {
		return 9;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
				rate = 1;
				break;
			}
			case 8: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch ((channel)) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
		}
		return rate;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = (80.0 / std::min<double>(192000.0, std::max<double>(1.0, double(fSampleRate))));
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.0);
		fCheckbox0 = FAUSTFLOAT(0.0);
		fCheckbox1 = FAUSTFLOAT(0.0);
		fVslider1 = FAUSTFLOAT(0.0);
		fCheckbox2 = FAUSTFLOAT(0.0);
		fCheckbox3 = FAUSTFLOAT(0.0);
		fCheckbox4 = FAUSTFLOAT(0.0);
		fCheckbox5 = FAUSTFLOAT(0.0);
		fCheckbox6 = FAUSTFLOAT(0.0);
		fCheckbox7 = FAUSTFLOAT(0.0);
		fCheckbox8 = FAUSTFLOAT(0.0);
		fCheckbox9 = FAUSTFLOAT(0.0);
		fCheckbox10 = FAUSTFLOAT(0.0);
		fCheckbox11 = FAUSTFLOAT(0.0);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
			fRec1[l0] = 0.0;
		}
		for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
			fRec3[l1] = 0.0;
		}
		for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
			fRec2[l2] = 0.0;
		}
		IOTA = 0;
		for (int l3 = 0; (l3 < 128); l3 = (l3 + 1)) {
			fVec0[l3] = 0.0;
		}
		for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
			fRec4[l4] = 0.0;
		}
		for (int l5 = 0; (l5 < 128); l5 = (l5 + 1)) {
			fVec1[l5] = 0.0;
		}
		for (int l6 = 0; (l6 < 2); l6 = (l6 + 1)) {
			fRec5[l6] = 0.0;
		}
		for (int l7 = 0; (l7 < 128); l7 = (l7 + 1)) {
			fVec2[l7] = 0.0;
		}
		for (int l8 = 0; (l8 < 2); l8 = (l8 + 1)) {
			fRec6[l8] = 0.0;
		}
		for (int l9 = 0; (l9 < 128); l9 = (l9 + 1)) {
			fVec3[l9] = 0.0;
		}
		for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
			fRec7[l10] = 0.0;
		}
		for (int l11 = 0; (l11 < 128); l11 = (l11 + 1)) {
			fVec4[l11] = 0.0;
		}
		for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
			fRec8[l12] = 0.0;
		}
		for (int l13 = 0; (l13 < 128); l13 = (l13 + 1)) {
			fVec5[l13] = 0.0;
		}
		for (int l14 = 0; (l14 < 2); l14 = (l14 + 1)) {
			fRec9[l14] = 0.0;
		}
		for (int l15 = 0; (l15 < 128); l15 = (l15 + 1)) {
			fVec6[l15] = 0.0;
		}
		for (int l16 = 0; (l16 < 2); l16 = (l16 + 1)) {
			fRec10[l16] = 0.0;
		}
		for (int l17 = 0; (l17 < 128); l17 = (l17 + 1)) {
			fVec7[l17] = 0.0;
		}
		for (int l18 = 0; (l18 < 2); l18 = (l18 + 1)) {
			fRec11[l18] = 0.0;
		}
		for (int l19 = 0; (l19 < 128); l19 = (l19 + 1)) {
			fVec8[l19] = 0.0;
		}
		for (int l20 = 0; (l20 < 2); l20 = (l20 + 1)) {
			fRec0[l20] = 0.0;
		}
		for (int l21 = 0; (l21 < 2); l21 = (l21 + 1)) {
			fRec12[l21] = 0.0;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual ue_binaural_decoder* clone() {
		return new ue_binaural_decoder();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Binaural decoder");
		ui_interface->openHorizontalBox("Inputs");
		ui_interface->openHorizontalBox("0");
		ui_interface->openVerticalBox("0");
		ui_interface->addCheckButton("Mute", &fCheckbox11);
		ui_interface->declare(&fVbargraph8, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86aec7ecc0", &fVbargraph8, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->addCheckButton("Mute", &fCheckbox10);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("1");
		ui_interface->openVerticalBox("1");
		ui_interface->addCheckButton("Mute", &fCheckbox8);
		ui_interface->declare(&fVbargraph6, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af00c9d0", &fVbargraph6, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("2");
		ui_interface->addCheckButton("Mute", &fCheckbox9);
		ui_interface->declare(&fVbargraph7, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af030f70", &fVbargraph7, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("3");
		ui_interface->addCheckButton("Mute", &fCheckbox7);
		ui_interface->declare(&fVbargraph5, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af2bdfb0", &fVbargraph5, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->addCheckButton("Mute", &fCheckbox6);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("2");
		ui_interface->openVerticalBox("4");
		ui_interface->addCheckButton("Mute", &fCheckbox5);
		ui_interface->declare(&fVbargraph4, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af28ef70", &fVbargraph4, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("5");
		ui_interface->addCheckButton("Mute", &fCheckbox4);
		ui_interface->declare(&fVbargraph3, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af24b780", &fVbargraph3, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("6");
		ui_interface->addCheckButton("Mute", &fCheckbox3);
		ui_interface->declare(&fVbargraph2, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af240d10", &fVbargraph2, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("7");
		ui_interface->addCheckButton("Mute", &fCheckbox2);
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af218080", &fVbargraph1, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("8");
		ui_interface->addCheckButton("Mute", &fCheckbox1);
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("0x7f86af20b8b0", &fVbargraph0, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->addCheckButton("Mute", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->declare(&fVslider1, "1", "");
		ui_interface->declare(&fVslider1, "osc", "/levelin -10 10");
		ui_interface->declare(&fVslider1, "unit", "dB");
		ui_interface->addVerticalSlider("Inputs Gain", &fVslider1, 0.0, -10.0, 10.0, 0.10000000000000001);
		ui_interface->declare(&fVslider0, "2", "");
		ui_interface->declare(&fVslider0, "osc", "/levelout -10 10");
		ui_interface->declare(&fVslider0, "unit", "dB");
		ui_interface->addVerticalSlider("Outputs Gain", &fVslider0, 0.0, -10.0, 10.0, 0.10000000000000001);
		ui_interface->closeBox();
		ui_interface->openVerticalBox("Outputs");
		ui_interface->openHorizontalBox("Left");
		ui_interface->declare(&fHbargraph0, "unit", "dB");
		ui_interface->addHorizontalBargraph("0x7f86af061850", &fHbargraph0, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Right");
		ui_interface->declare(&fHbargraph1, "unit", "dB");
		ui_interface->addHorizontalBargraph("0x7f86af5b81e0", &fHbargraph1, -70.0, 6.0);
		ui_interface->closeBox();
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* input6 = inputs[6];
		FAUSTFLOAT* input7 = inputs[7];
		FAUSTFLOAT* input8 = inputs[8];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		double fSlow0 = (0.0010000000000000009 * std::pow(10.0, (0.050000000000000003 * double(fVslider0))));
		double fSlow1 = (1.0 - double(fCheckbox0));
		double fSlow2 = (fSlow1 * (1.0 - double(fCheckbox1)));
		double fSlow3 = (0.0010000000000000009 * std::pow(10.0, (0.050000000000000003 * double(fVslider1))));
		double fSlow4 = (fSlow1 * (1.0 - double(fCheckbox2)));
		double fSlow5 = (fSlow1 * (1.0 - double(fCheckbox3)));
		double fSlow6 = (fSlow1 * (1.0 - double(fCheckbox4)));
		double fSlow7 = (fSlow1 * (1.0 - double(fCheckbox5)));
		double fSlow8 = (1.0 - double(fCheckbox6));
		double fSlow9 = (fSlow8 * (1.0 - double(fCheckbox7)));
		double fSlow10 = (fSlow8 * (1.0 - double(fCheckbox8)));
		double fSlow11 = (fSlow8 * (1.0 - double(fCheckbox9)));
		double fSlow12 = ((1.0 - double(fCheckbox10)) * (1.0 - double(fCheckbox11)));
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec1[0] = (fSlow0 + (0.999 * fRec1[1]));
			fRec3[0] = (fSlow3 + (0.999 * fRec3[1]));
			double fTemp0 = (fSlow2 * (double(input8[i]) * fRec3[0]));
			fRec2[0] = std::max<double>((fRec2[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp0))))));
			fVbargraph0 = FAUSTFLOAT(fRec2[0]);
			fVec0[(IOTA & 127)] = fTemp0;
			double fTemp1 = fVec0[((IOTA - 121) & 127)];
			double fTemp2 = fVec0[((IOTA - 110) & 127)];
			double fTemp3 = fVec0[((IOTA - 75) & 127)];
			double fTemp4 = fVec0[((IOTA - 74) & 127)];
			double fTemp5 = fVec0[((IOTA - 70) & 127)];
			double fTemp6 = fVec0[((IOTA - 72) & 127)];
			double fTemp7 = fVec0[((IOTA - 71) & 127)];
			double fTemp8 = fVec0[((IOTA - 53) & 127)];
			double fTemp9 = fVec0[((IOTA - 42) & 127)];
			double fTemp10 = fVec0[((IOTA - 38) & 127)];
			double fTemp11 = (fSlow4 * (double(input7[i]) * fRec3[0]));
			fRec4[0] = std::max<double>((fRec4[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp11))))));
			fVbargraph1 = FAUSTFLOAT(fRec4[0]);
			fVec1[(IOTA & 127)] = fTemp11;
			double fTemp12 = fVec1[((IOTA - 125) & 127)];
			double fTemp13 = fVec1[((IOTA - 117) & 127)];
			double fTemp14 = fVec1[((IOTA - 116) & 127)];
			double fTemp15 = fVec1[((IOTA - 121) & 127)];
			double fTemp16 = fVec1[((IOTA - 111) & 127)];
			double fTemp17 = fVec1[((IOTA - 110) & 127)];
			double fTemp18 = fVec1[((IOTA - 103) & 127)];
			double fTemp19 = fVec1[((IOTA - 102) & 127)];
			double fTemp20 = fVec1[((IOTA - 101) & 127)];
			double fTemp21 = fVec1[((IOTA - 94) & 127)];
			double fTemp22 = fVec1[((IOTA - 100) & 127)];
			double fTemp23 = fVec1[((IOTA - 99) & 127)];
			double fTemp24 = fVec1[((IOTA - 93) & 127)];
			double fTemp25 = fVec1[((IOTA - 77) & 127)];
			double fTemp26 = fVec1[((IOTA - 76) & 127)];
			double fTemp27 = fVec1[((IOTA - 68) & 127)];
			double fTemp28 = fVec1[((IOTA - 72) & 127)];
			double fTemp29 = fVec1[((IOTA - 70) & 127)];
			double fTemp30 = fVec1[((IOTA - 67) & 127)];
			double fTemp31 = fVec1[((IOTA - 66) & 127)];
			double fTemp32 = fVec1[((IOTA - 59) & 127)];
			double fTemp33 = fVec1[((IOTA - 44) & 127)];
			double fTemp34 = fVec1[((IOTA - 41) & 127)];
			double fTemp35 = fVec1[((IOTA - 32) & 127)];
			double fTemp36 = fVec1[((IOTA - 31) & 127)];
			double fTemp37 = fVec1[((IOTA - 29) & 127)];
			double fTemp38 = fVec1[((IOTA - 26) & 127)];
			double fTemp39 = fVec1[((IOTA - 25) & 127)];
			double fTemp40 = fVec0[((IOTA - 126) & 127)];
			double fTemp41 = fVec0[((IOTA - 122) & 127)];
			double fTemp42 = fVec0[((IOTA - 95) & 127)];
			double fTemp43 = fVec0[((IOTA - 96) & 127)];
			double fTemp44 = fVec0[((IOTA - 94) & 127)];
			double fTemp45 = fVec0[((IOTA - 91) & 127)];
			double fTemp46 = fVec0[((IOTA - 104) & 127)];
			double fTemp47 = fVec0[((IOTA - 103) & 127)];
			double fTemp48 = fVec0[((IOTA - 77) & 127)];
			double fTemp49 = fVec0[((IOTA - 79) & 127)];
			double fTemp50 = fVec0[((IOTA - 76) & 127)];
			double fTemp51 = fVec0[((IOTA - 68) & 127)];
			double fTemp52 = fVec0[((IOTA - 67) & 127)];
			double fTemp53 = fVec0[((IOTA - 66) & 127)];
			double fTemp54 = fVec0[((IOTA - 65) & 127)];
			double fTemp55 = fVec0[((IOTA - 64) & 127)];
			double fTemp56 = fVec0[((IOTA - 20) & 127)];
			double fTemp57 = fVec0[((IOTA - 19) & 127)];
			double fTemp58 = fVec0[((IOTA - 18) & 127)];
			double fTemp59 = fVec0[((IOTA - 10) & 127)];
			double fTemp60 = fVec0[((IOTA - 8) & 127)];
			double fTemp61 = fVec0[((IOTA - 6) & 127)];
			double fTemp62 = fVec0[((IOTA - 4) & 127)];
			double fTemp63 = fVec0[((IOTA - 2) & 127)];
			double fTemp64 = fVec1[((IOTA - 126) & 127)];
			double fTemp65 = fVec1[((IOTA - 104) & 127)];
			double fTemp66 = fVec1[((IOTA - 106) & 127)];
			double fTemp67 = fVec1[((IOTA - 105) & 127)];
			double fTemp68 = fVec1[((IOTA - 98) & 127)];
			double fTemp69 = fVec1[((IOTA - 97) & 127)];
			double fTemp70 = fVec1[((IOTA - 50) & 127)];
			double fTemp71 = (fSlow5 * (double(input6[i]) * fRec3[0]));
			fRec5[0] = std::max<double>((fRec5[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp71))))));
			fVbargraph2 = FAUSTFLOAT(fRec5[0]);
			fVec2[(IOTA & 127)] = fTemp71;
			double fTemp72 = fVec2[((IOTA - 94) & 127)];
			double fTemp73 = fVec2[((IOTA - 93) & 127)];
			double fTemp74 = fVec2[((IOTA - 92) & 127)];
			double fTemp75 = fVec2[((IOTA - 110) & 127)];
			double fTemp76 = fVec2[((IOTA - 91) & 127)];
			double fTemp77 = fVec2[((IOTA - 109) & 127)];
			double fTemp78 = fVec2[((IOTA - 90) & 127)];
			double fTemp79 = fVec2[((IOTA - 22) & 127)];
			double fTemp80 = (fSlow6 * (double(input5[i]) * fRec3[0]));
			fRec6[0] = std::max<double>((fRec6[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp80))))));
			fVbargraph3 = FAUSTFLOAT(fRec6[0]);
			fVec3[(IOTA & 127)] = fTemp80;
			double fTemp81 = fVec3[((IOTA - 123) & 127)];
			double fTemp82 = fVec2[((IOTA - 24) & 127)];
			double fTemp83 = fVec3[((IOTA - 107) & 127)];
			double fTemp84 = fVec3[((IOTA - 104) & 127)];
			double fTemp85 = fVec3[((IOTA - 103) & 127)];
			double fTemp86 = fVec3[((IOTA - 102) & 127)];
			double fTemp87 = fVec3[((IOTA - 106) & 127)];
			double fTemp88 = fVec3[((IOTA - 105) & 127)];
			double fTemp89 = fVec2[((IOTA - 26) & 127)];
			double fTemp90 = fVec2[((IOTA - 25) & 127)];
			double fTemp91 = fVec3[((IOTA - 99) & 127)];
			double fTemp92 = fVec1[((IOTA - 18) & 127)];
			double fTemp93 = fVec1[((IOTA - 19) & 127)];
			double fTemp94 = fVec1[((IOTA - 15) & 127)];
			double fTemp95 = fVec1[((IOTA - 12) & 127)];
			double fTemp96 = fVec1[((IOTA - 11) & 127)];
			double fTemp97 = fVec1[((IOTA - 9) & 127)];
			double fTemp98 = fVec1[((IOTA - 10) & 127)];
			double fTemp99 = fVec1[((IOTA - 3) & 127)];
			double fTemp100 = fVec1[((IOTA - 2) & 127)];
			double fTemp101 = fVec1[((IOTA - 7) & 127)];
			double fTemp102 = fVec1[((IOTA - 6) & 127)];
			double fTemp103 = fVec2[((IOTA - 118) & 127)];
			double fTemp104 = fVec2[((IOTA - 108) & 127)];
			double fTemp105 = fVec2[((IOTA - 107) & 127)];
			double fTemp106 = fVec2[((IOTA - 105) & 127)];
			double fTemp107 = fVec2[((IOTA - 104) & 127)];
			double fTemp108 = fVec2[((IOTA - 102) & 127)];
			double fTemp109 = fVec2[((IOTA - 101) & 127)];
			double fTemp110 = fVec2[((IOTA - 100) & 127)];
			double fTemp111 = fVec2[((IOTA - 99) & 127)];
			double fTemp112 = fVec2[((IOTA - 98) & 127)];
			double fTemp113 = fVec2[((IOTA - 106) & 127)];
			double fTemp114 = fVec2[((IOTA - 119) & 127)];
			double fTemp115 = fVec2[((IOTA - 126) & 127)];
			double fTemp116 = fVec2[((IOTA - 88) & 127)];
			double fTemp117 = fVec2[((IOTA - 97) & 127)];
			double fTemp118 = fVec2[((IOTA - 87) & 127)];
			double fTemp119 = fVec2[((IOTA - 84) & 127)];
			double fTemp120 = fVec2[((IOTA - 83) & 127)];
			double fTemp121 = fVec2[((IOTA - 79) & 127)];
			double fTemp122 = fVec2[((IOTA - 82) & 127)];
			double fTemp123 = fVec2[((IOTA - 75) & 127)];
			double fTemp124 = fVec2[((IOTA - 72) & 127)];
			double fTemp125 = fVec2[((IOTA - 71) & 127)];
			double fTemp126 = fVec2[((IOTA - 70) & 127)];
			double fTemp127 = fVec2[((IOTA - 66) & 127)];
			double fTemp128 = fVec2[((IOTA - 64) & 127)];
			double fTemp129 = fVec2[((IOTA - 60) & 127)];
			double fTemp130 = fVec2[((IOTA - 52) & 127)];
			double fTemp131 = fVec2[((IOTA - 63) & 127)];
			double fTemp132 = fVec2[((IOTA - 65) & 127)];
			double fTemp133 = fVec3[((IOTA - 126) & 127)];
			double fTemp134 = fVec3[((IOTA - 125) & 127)];
			double fTemp135 = fVec3[((IOTA - 124) & 127)];
			double fTemp136 = fVec3[((IOTA - 116) & 127)];
			double fTemp137 = fVec3[((IOTA - 113) & 127)];
			double fTemp138 = fVec3[((IOTA - 110) & 127)];
			double fTemp139 = fVec3[((IOTA - 108) & 127)];
			double fTemp140 = fVec3[((IOTA - 109) & 127)];
			double fTemp141 = fVec3[((IOTA - 98) & 127)];
			double fTemp142 = fVec3[((IOTA - 97) & 127)];
			double fTemp143 = fVec3[((IOTA - 96) & 127)];
			double fTemp144 = fVec3[((IOTA - 95) & 127)];
			double fTemp145 = fVec3[((IOTA - 94) & 127)];
			double fTemp146 = fVec3[((IOTA - 93) & 127)];
			double fTemp147 = fVec3[((IOTA - 92) & 127)];
			double fTemp148 = fVec3[((IOTA - 91) & 127)];
			double fTemp149 = fVec3[((IOTA - 90) & 127)];
			double fTemp150 = fVec3[((IOTA - 89) & 127)];
			double fTemp151 = fVec3[((IOTA - 88) & 127)];
			double fTemp152 = fVec3[((IOTA - 115) & 127)];
			double fTemp153 = fVec3[((IOTA - 114) & 127)];
			double fTemp154 = fVec3[((IOTA - 86) & 127)];
			double fTemp155 = fVec3[((IOTA - 85) & 127)];
			double fTemp156 = fVec3[((IOTA - 83) & 127)];
			double fTemp157 = fVec3[((IOTA - 77) & 127)];
			double fTemp158 = fVec3[((IOTA - 76) & 127)];
			double fTemp159 = fVec3[((IOTA - 87) & 127)];
			double fTemp160 = fVec3[((IOTA - 75) & 127)];
			double fTemp161 = fVec3[((IOTA - 78) & 127)];
			double fTemp162 = fVec3[((IOTA - 74) & 127)];
			double fTemp163 = fVec3[((IOTA - 73) & 127)];
			double fTemp164 = fVec3[((IOTA - 71) & 127)];
			double fTemp165 = fVec3[((IOTA - 72) & 127)];
			double fTemp166 = fVec3[((IOTA - 64) & 127)];
			double fTemp167 = fVec3[((IOTA - 60) & 127)];
			double fTemp168 = fVec3[((IOTA - 35) & 127)];
			double fTemp169 = fVec3[((IOTA - 38) & 127)];
			double fTemp170 = fVec3[((IOTA - 33) & 127)];
			double fTemp171 = fVec3[((IOTA - 32) & 127)];
			double fTemp172 = fVec3[((IOTA - 36) & 127)];
			double fTemp173 = fVec3[((IOTA - 29) & 127)];
			double fTemp174 = fVec3[((IOTA - 28) & 127)];
			double fTemp175 = fVec3[((IOTA - 23) & 127)];
			double fTemp176 = fVec3[((IOTA - 24) & 127)];
			double fTemp177 = fVec3[((IOTA - 21) & 127)];
			double fTemp178 = fVec3[((IOTA - 22) & 127)];
			double fTemp179 = fVec3[((IOTA - 20) & 127)];
			double fTemp180 = fVec3[((IOTA - 17) & 127)];
			double fTemp181 = fVec3[((IOTA - 13) & 127)];
			double fTemp182 = fVec3[((IOTA - 14) & 127)];
			double fTemp183 = fVec3[((IOTA - 10) & 127)];
			double fTemp184 = fVec3[((IOTA - 8) & 127)];
			double fTemp185 = fVec3[((IOTA - 6) & 127)];
			double fTemp186 = fVec3[((IOTA - 5) & 127)];
			double fTemp187 = fVec3[((IOTA - 4) & 127)];
			double fTemp188 = fVec3[((IOTA - 2) & 127)];
			double fTemp189 = fVec3[((IOTA - 1) & 127)];
			double fTemp190 = fVec3[((IOTA - 39) & 127)];
			double fTemp191 = (fSlow7 * (double(input4[i]) * fRec3[0]));
			fRec7[0] = std::max<double>((fRec7[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp191))))));
			fVbargraph4 = FAUSTFLOAT(fRec7[0]);
			fVec4[(IOTA & 127)] = fTemp191;
			double fTemp192 = fVec4[((IOTA - 119) & 127)];
			double fTemp193 = fVec3[((IOTA - 40) & 127)];
			double fTemp194 = fVec4[((IOTA - 117) & 127)];
			double fTemp195 = fVec4[((IOTA - 116) & 127)];
			double fTemp196 = fVec4[((IOTA - 113) & 127)];
			double fTemp197 = fVec4[((IOTA - 125) & 127)];
			double fTemp198 = fVec4[((IOTA - 114) & 127)];
			double fTemp199 = fVec4[((IOTA - 111) & 127)];
			double fTemp200 = fVec4[((IOTA - 110) & 127)];
			double fTemp201 = fVec4[((IOTA - 112) & 127)];
			double fTemp202 = fVec4[((IOTA - 106) & 127)];
			double fTemp203 = fVec4[((IOTA - 105) & 127)];
			double fTemp204 = fVec4[((IOTA - 108) & 127)];
			double fTemp205 = fVec4[((IOTA - 104) & 127)];
			double fTemp206 = fVec4[((IOTA - 126) & 127)];
			double fTemp207 = fVec4[((IOTA - 124) & 127)];
			double fTemp208 = fVec4[((IOTA - 101) & 127)];
			double fTemp209 = fVec4[((IOTA - 98) & 127)];
			double fTemp210 = fVec4[((IOTA - 97) & 127)];
			double fTemp211 = fVec4[((IOTA - 93) & 127)];
			double fTemp212 = fVec4[((IOTA - 95) & 127)];
			double fTemp213 = fVec4[((IOTA - 87) & 127)];
			double fTemp214 = fVec4[((IOTA - 86) & 127)];
			double fTemp215 = fVec4[((IOTA - 75) & 127)];
			double fTemp216 = fVec4[((IOTA - 83) & 127)];
			double fTemp217 = fVec4[((IOTA - 94) & 127)];
			double fTemp218 = fVec4[((IOTA - 107) & 127)];
			double fTemp219 = fVec4[((IOTA - 74) & 127)];
			double fTemp220 = fVec4[((IOTA - 62) & 127)];
			double fTemp221 = fVec4[((IOTA - 50) & 127)];
			double fTemp222 = fVec4[((IOTA - 48) & 127)];
			double fTemp223 = fVec4[((IOTA - 49) & 127)];
			double fTemp224 = fVec4[((IOTA - 45) & 127)];
			double fTemp225 = fVec4[((IOTA - 42) & 127)];
			double fTemp226 = fVec4[((IOTA - 44) & 127)];
			double fTemp227 = fVec4[((IOTA - 40) & 127)];
			double fTemp228 = fVec4[((IOTA - 38) & 127)];
			double fTemp229 = fVec4[((IOTA - 39) & 127)];
			double fTemp230 = fVec4[((IOTA - 37) & 127)];
			double fTemp231 = fVec4[((IOTA - 36) & 127)];
			double fTemp232 = fVec4[((IOTA - 35) & 127)];
			double fTemp233 = fVec4[((IOTA - 31) & 127)];
			double fTemp234 = fVec4[((IOTA - 30) & 127)];
			double fTemp235 = fVec0[((IOTA - 117) & 127)];
			double fTemp236 = fVec0[((IOTA - 33) & 127)];
			double fTemp237 = fVec0[((IOTA - 34) & 127)];
			double fTemp238 = fVec1[((IOTA - 64) & 127)];
			double fTemp239 = fVec1[((IOTA - 86) & 127)];
			double fTemp240 = fVec1[((IOTA - 85) & 127)];
			double fTemp241 = fVec1[((IOTA - 63) & 127)];
			double fTemp242 = fVec1[((IOTA - 62) & 127)];
			double fTemp243 = fVec1[((IOTA - 58) & 127)];
			double fTemp244 = fVec1[((IOTA - 57) & 127)];
			double fTemp245 = fVec1[((IOTA - 54) & 127)];
			double fTemp246 = fVec1[((IOTA - 55) & 127)];
			double fTemp247 = fVec1[((IOTA - 56) & 127)];
			double fTemp248 = fVec1[((IOTA - 36) & 127)];
			double fTemp249 = fVec1[((IOTA - 38) & 127)];
			double fTemp250 = fVec1[((IOTA - 35) & 127)];
			double fTemp251 = fVec2[((IOTA - 20) & 127)];
			double fTemp252 = fVec2[((IOTA - 21) & 127)];
			double fTemp253 = fVec2[((IOTA - 19) & 127)];
			double fTemp254 = fVec2[((IOTA - 18) & 127)];
			double fTemp255 = fVec2[((IOTA - 10) & 127)];
			double fTemp256 = fVec2[((IOTA - 8) & 127)];
			double fTemp257 = fVec2[((IOTA - 6) & 127)];
			double fTemp258 = fVec2[((IOTA - 2) & 127)];
			double fTemp259 = fVec4[((IOTA - 123) & 127)];
			double fTemp260 = fVec4[((IOTA - 92) & 127)];
			double fTemp261 = fVec4[((IOTA - 78) & 127)];
			double fTemp262 = fVec4[((IOTA - 59) & 127)];
			double fTemp263 = fVec4[((IOTA - 58) & 127)];
			double fTemp264 = fVec4[((IOTA - 52) & 127)];
			double fTemp265 = (fSlow9 * (double(input3[i]) * fRec3[0]));
			fRec8[0] = std::max<double>((fRec8[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp265))))));
			fVbargraph5 = FAUSTFLOAT(fRec8[0]);
			fVec5[(IOTA & 127)] = fTemp265;
			double fTemp266 = fVec5[((IOTA - 125) & 127)];
			double fTemp267 = fVec4[((IOTA - 57) & 127)];
			double fTemp268 = fVec4[((IOTA - 56) & 127)];
			double fTemp269 = fVec5[((IOTA - 123) & 127)];
			double fTemp270 = fVec0[((IOTA - 120) & 127)];
			double fTemp271 = fVec0[((IOTA - 86) & 127)];
			double fTemp272 = fVec0[((IOTA - 69) & 127)];
			double fTemp273 = fVec0[((IOTA - 28) & 127)];
			double fTemp274 = fVec0[((IOTA - 30) & 127)];
			double fTemp275 = fVec0[((IOTA - 29) & 127)];
			double fTemp276 = fVec0[((IOTA - 27) & 127)];
			double fTemp277 = fVec0[((IOTA - 24) & 127)];
			double fTemp278 = fVec0[((IOTA - 23) & 127)];
			double fTemp279 = fVec0[((IOTA - 26) & 127)];
			double fTemp280 = fVec1[((IOTA - 108) & 127)];
			double fTemp281 = fVec1[((IOTA - 107) & 127)];
			double fTemp282 = fVec1[((IOTA - 71) & 127)];
			double fTemp283 = fVec1[((IOTA - 61) & 127)];
			double fTemp284 = fVec1[((IOTA - 51) & 127)];
			double fTemp285 = fVec3[((IOTA - 67) & 127)];
			double fTemp286 = (fSlow10 * (double(input1[i]) * fRec3[0]));
			fRec9[0] = std::max<double>((fRec9[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp286))))));
			fVbargraph6 = FAUSTFLOAT(fRec9[0]);
			fVec6[(IOTA & 127)] = fTemp286;
			double fTemp287 = fVec6[((IOTA - 99) & 127)];
			double fTemp288 = fVec4[((IOTA - 61) & 127)];
			double fTemp289 = fVec4[((IOTA - 4) & 127)];
			double fTemp290 = fVec4[((IOTA - 2) & 127)];
			double fTemp291 = fVec4[((IOTA - 1) & 127)];
			double fTemp292 = fVec4[((IOTA - 18) & 127)];
			double fTemp293 = fVec4[((IOTA - 17) & 127)];
			double fTemp294 = fVec4[((IOTA - 16) & 127)];
			double fTemp295 = fVec4[((IOTA - 15) & 127)];
			double fTemp296 = fVec4[((IOTA - 13) & 127)];
			double fTemp297 = fVec4[((IOTA - 10) & 127)];
			double fTemp298 = fVec4[((IOTA - 8) & 127)];
			double fTemp299 = fVec4[((IOTA - 6) & 127)];
			double fTemp300 = fVec5[((IOTA - 121) & 127)];
			double fTemp301 = fVec5[((IOTA - 126) & 127)];
			double fTemp302 = fVec4[((IOTA - 26) & 127)];
			double fTemp303 = fVec4[((IOTA - 25) & 127)];
			double fTemp304 = fVec5[((IOTA - 120) & 127)];
			double fTemp305 = fVec5[((IOTA - 119) & 127)];
			double fTemp306 = fVec5[((IOTA - 117) & 127)];
			double fTemp307 = fVec5[((IOTA - 116) & 127)];
			double fTemp308 = fVec5[((IOTA - 95) & 127)];
			double fTemp309 = fVec5[((IOTA - 96) & 127)];
			double fTemp310 = fVec5[((IOTA - 94) & 127)];
			double fTemp311 = fVec5[((IOTA - 91) & 127)];
			double fTemp312 = fVec5[((IOTA - 90) & 127)];
			double fTemp313 = fVec5[((IOTA - 85) & 127)];
			double fTemp314 = fVec5[((IOTA - 84) & 127)];
			double fTemp315 = fVec5[((IOTA - 87) & 127)];
			double fTemp316 = fVec5[((IOTA - 83) & 127)];
			double fTemp317 = fVec5[((IOTA - 64) & 127)];
			double fTemp318 = fVec5[((IOTA - 62) & 127)];
			double fTemp319 = fVec5[((IOTA - 57) & 127)];
			double fTemp320 = fVec5[((IOTA - 56) & 127)];
			double fTemp321 = fVec5[((IOTA - 63) & 127)];
			double fTemp322 = fVec5[((IOTA - 65) & 127)];
			double fTemp323 = fVec5[((IOTA - 52) & 127)];
			double fTemp324 = fVec5[((IOTA - 51) & 127)];
			double fTemp325 = fVec5[((IOTA - 53) & 127)];
			double fTemp326 = fVec5[((IOTA - 55) & 127)];
			double fTemp327 = fVec5[((IOTA - 54) & 127)];
			double fTemp328 = fVec5[((IOTA - 43) & 127)];
			double fTemp329 = fVec5[((IOTA - 49) & 127)];
			double fTemp330 = fVec5[((IOTA - 48) & 127)];
			double fTemp331 = fVec5[((IOTA - 42) & 127)];
			double fTemp332 = fVec5[((IOTA - 39) & 127)];
			double fTemp333 = fVec5[((IOTA - 37) & 127)];
			double fTemp334 = fVec5[((IOTA - 36) & 127)];
			double fTemp335 = fVec5[((IOTA - 33) & 127)];
			double fTemp336 = fVec5[((IOTA - 34) & 127)];
			double fTemp337 = fVec5[((IOTA - 38) & 127)];
			double fTemp338 = fVec5[((IOTA - 29) & 127)];
			double fTemp339 = fVec5[((IOTA - 26) & 127)];
			double fTemp340 = fVec5[((IOTA - 23) & 127)];
			double fTemp341 = fVec5[((IOTA - 22) & 127)];
			double fTemp342 = fVec5[((IOTA - 45) & 127)];
			double fTemp343 = fVec5[((IOTA - 44) & 127)];
			double fTemp344 = (fSlow11 * (double(input2[i]) * fRec3[0]));
			fRec10[0] = std::max<double>((fRec10[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp344))))));
			fVbargraph7 = FAUSTFLOAT(fRec10[0]);
			fVec7[(IOTA & 127)] = fTemp344;
			double fTemp345 = fVec7[((IOTA - 122) & 127)];
			double fTemp346 = fVec7[((IOTA - 121) & 127)];
			double fTemp347 = fVec7[((IOTA - 119) & 127)];
			double fTemp348 = fVec7[((IOTA - 120) & 127)];
			double fTemp349 = fVec7[((IOTA - 118) & 127)];
			double fTemp350 = fVec7[((IOTA - 116) & 127)];
			double fTemp351 = fVec7[((IOTA - 117) & 127)];
			double fTemp352 = fVec7[((IOTA - 125) & 127)];
			double fTemp353 = fVec7[((IOTA - 98) & 127)];
			double fTemp354 = fVec7[((IOTA - 97) & 127)];
			double fTemp355 = fVec7[((IOTA - 94) & 127)];
			double fTemp356 = fVec7[((IOTA - 93) & 127)];
			double fTemp357 = fVec7[((IOTA - 90) & 127)];
			double fTemp358 = fVec7[((IOTA - 92) & 127)];
			double fTemp359 = fVec7[((IOTA - 89) & 127)];
			double fTemp360 = fVec7[((IOTA - 88) & 127)];
			double fTemp361 = fVec7[((IOTA - 87) & 127)];
			double fTemp362 = fVec7[((IOTA - 86) & 127)];
			double fTemp363 = fVec7[((IOTA - 85) & 127)];
			double fTemp364 = fVec7[((IOTA - 77) & 127)];
			double fTemp365 = fVec7[((IOTA - 78) & 127)];
			double fTemp366 = fVec7[((IOTA - 75) & 127)];
			double fTemp367 = fVec7[((IOTA - 84) & 127)];
			double fTemp368 = fVec7[((IOTA - 62) & 127)];
			double fTemp369 = fVec7[((IOTA - 63) & 127)];
			double fTemp370 = fVec7[((IOTA - 58) & 127)];
			double fTemp371 = fVec7[((IOTA - 60) & 127)];
			double fTemp372 = fVec7[((IOTA - 59) & 127)];
			double fTemp373 = fVec7[((IOTA - 83) & 127)];
			double fTemp374 = fVec7[((IOTA - 54) & 127)];
			double fTemp375 = fVec7[((IOTA - 52) & 127)];
			double fTemp376 = fVec7[((IOTA - 48) & 127)];
			double fTemp377 = fVec7[((IOTA - 47) & 127)];
			double fTemp378 = fVec7[((IOTA - 53) & 127)];
			double fTemp379 = fVec7[((IOTA - 50) & 127)];
			double fTemp380 = fVec7[((IOTA - 51) & 127)];
			double fTemp381 = fVec7[((IOTA - 45) & 127)];
			double fTemp382 = fVec7[((IOTA - 42) & 127)];
			double fTemp383 = fVec7[((IOTA - 46) & 127)];
			double fTemp384 = fVec7[((IOTA - 44) & 127)];
			double fTemp385 = fVec7[((IOTA - 40) & 127)];
			double fTemp386 = fVec7[((IOTA - 39) & 127)];
			double fTemp387 = fVec7[((IOTA - 41) & 127)];
			double fTemp388 = fVec7[((IOTA - 33) & 127)];
			double fTemp389 = fVec7[((IOTA - 36) & 127)];
			double fTemp390 = fVec7[((IOTA - 35) & 127)];
			double fTemp391 = fVec7[((IOTA - 32) & 127)];
			double fTemp392 = fVec7[((IOTA - 29) & 127)];
			double fTemp393 = fVec7[((IOTA - 28) & 127)];
			double fTemp394 = fVec7[((IOTA - 24) & 127)];
			double fTemp395 = fVec7[((IOTA - 26) & 127)];
			double fTemp396 = fVec7[((IOTA - 21) & 127)];
			double fTemp397 = fVec7[((IOTA - 20) & 127)];
			double fTemp398 = fVec7[((IOTA - 17) & 127)];
			double fTemp399 = fVec7[((IOTA - 14) & 127)];
			double fTemp400 = fVec7[((IOTA - 13) & 127)];
			double fTemp401 = fVec7[((IOTA - 8) & 127)];
			double fTemp402 = fVec7[((IOTA - 5) & 127)];
			double fTemp403 = fVec7[((IOTA - 4) & 127)];
			double fTemp404 = fVec7[((IOTA - 2) & 127)];
			double fTemp405 = fVec7[((IOTA - 1) & 127)];
			double fTemp406 = fVec6[((IOTA - 114) & 127)];
			double fTemp407 = fVec6[((IOTA - 105) & 127)];
			double fTemp408 = fVec6[((IOTA - 103) & 127)];
			double fTemp409 = fVec6[((IOTA - 102) & 127)];
			double fTemp410 = fVec6[((IOTA - 104) & 127)];
			double fTemp411 = fVec6[((IOTA - 98) & 127)];
			double fTemp412 = fVec6[((IOTA - 95) & 127)];
			double fTemp413 = fVec6[((IOTA - 92) & 127)];
			double fTemp414 = fVec6[((IOTA - 90) & 127)];
			double fTemp415 = fVec6[((IOTA - 101) & 127)];
			double fTemp416 = fVec6[((IOTA - 100) & 127)];
			double fTemp417 = fVec6[((IOTA - 88) & 127)];
			double fTemp418 = fVec6[((IOTA - 87) & 127)];
			double fTemp419 = fVec6[((IOTA - 86) & 127)];
			double fTemp420 = fVec6[((IOTA - 85) & 127)];
			double fTemp421 = fVec6[((IOTA - 97) & 127)];
			double fTemp422 = fVec6[((IOTA - 84) & 127)];
			double fTemp423 = fVec6[((IOTA - 83) & 127)];
			double fTemp424 = fVec6[((IOTA - 94) & 127)];
			double fTemp425 = fVec6[((IOTA - 80) & 127)];
			double fTemp426 = fVec6[((IOTA - 79) & 127)];
			double fTemp427 = fVec6[((IOTA - 77) & 127)];
			double fTemp428 = fVec6[((IOTA - 89) & 127)];
			double fTemp429 = fVec6[((IOTA - 96) & 127)];
			double fTemp430 = fVec6[((IOTA - 82) & 127)];
			double fTemp431 = fVec6[((IOTA - 93) & 127)];
			double fTemp432 = (fSlow12 * (double(input0[i]) * fRec3[0]));
			fRec11[0] = std::max<double>((fRec11[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp432))))));
			fVbargraph8 = FAUSTFLOAT(fRec11[0]);
			fVec8[(IOTA & 127)] = fTemp432;
			double fTemp433 = fVec8[((IOTA - 125) & 127)];
			double fTemp434 = fVec8[((IOTA - 121) & 127)];
			double fTemp435 = fVec8[((IOTA - 124) & 127)];
			double fTemp436 = fVec8[((IOTA - 119) & 127)];
			double fTemp437 = fVec8[((IOTA - 120) & 127)];
			double fTemp438 = fVec8[((IOTA - 122) & 127)];
			double fTemp439 = fVec8[((IOTA - 118) & 127)];
			double fTemp440 = fVec8[((IOTA - 111) & 127)];
			double fTemp441 = fVec8[((IOTA - 113) & 127)];
			double fTemp442 = fVec8[((IOTA - 109) & 127)];
			double fTemp443 = fVec8[((IOTA - 110) & 127)];
			double fTemp444 = fVec8[((IOTA - 112) & 127)];
			double fTemp445 = fVec8[((IOTA - 114) & 127)];
			double fTemp446 = fVec8[((IOTA - 108) & 127)];
			double fTemp447 = fVec8[((IOTA - 105) & 127)];
			double fTemp448 = fVec8[((IOTA - 104) & 127)];
			double fTemp449 = fVec8[((IOTA - 102) & 127)];
			double fTemp450 = fVec8[((IOTA - 103) & 127)];
			double fTemp451 = fVec8[((IOTA - 98) & 127)];
			double fTemp452 = fVec8[((IOTA - 100) & 127)];
			double fTemp453 = fVec8[((IOTA - 101) & 127)];
			double fTemp454 = fVec8[((IOTA - 96) & 127)];
			double fTemp455 = fVec8[((IOTA - 95) & 127)];
			double fTemp456 = fVec8[((IOTA - 93) & 127)];
			double fTemp457 = fVec8[((IOTA - 94) & 127)];
			double fTemp458 = fVec8[((IOTA - 99) & 127)];
			double fTemp459 = fVec8[((IOTA - 87) & 127)];
			double fTemp460 = fVec8[((IOTA - 85) & 127)];
			double fTemp461 = fVec8[((IOTA - 86) & 127)];
			double fTemp462 = fVec8[((IOTA - 90) & 127)];
			double fTemp463 = fVec8[((IOTA - 76) & 127)];
			double fTemp464 = fVec8[((IOTA - 82) & 127)];
			double fTemp465 = fVec8[((IOTA - 83) & 127)];
			double fTemp466 = fVec8[((IOTA - 75) & 127)];
			double fTemp467 = fVec8[((IOTA - 72) & 127)];
			double fTemp468 = fVec8[((IOTA - 74) & 127)];
			double fTemp469 = fVec8[((IOTA - 71) & 127)];
			double fTemp470 = fVec8[((IOTA - 73) & 127)];
			double fTemp471 = fVec8[((IOTA - 70) & 127)];
			double fTemp472 = fVec8[((IOTA - 69) & 127)];
			double fTemp473 = fVec8[((IOTA - 68) & 127)];
			double fTemp474 = fVec8[((IOTA - 67) & 127)];
			double fTemp475 = fVec8[((IOTA - 66) & 127)];
			double fTemp476 = fVec8[((IOTA - 65) & 127)];
			double fTemp477 = fVec8[((IOTA - 64) & 127)];
			double fTemp478 = fVec8[((IOTA - 62) & 127)];
			double fTemp479 = fVec8[((IOTA - 63) & 127)];
			double fTemp480 = fVec8[((IOTA - 61) & 127)];
			double fTemp481 = fVec8[((IOTA - 60) & 127)];
			double fTemp482 = fVec8[((IOTA - 59) & 127)];
			double fTemp483 = fVec8[((IOTA - 55) & 127)];
			double fTemp484 = fVec8[((IOTA - 56) & 127)];
			double fTemp485 = fVec8[((IOTA - 54) & 127)];
			double fTemp486 = fVec8[((IOTA - 53) & 127)];
			double fTemp487 = fVec8[((IOTA - 52) & 127)];
			double fTemp488 = fVec8[((IOTA - 44) & 127)];
			double fTemp489 = fVec8[((IOTA - 45) & 127)];
			double fTemp490 = fVec8[((IOTA - 43) & 127)];
			double fTemp491 = fVec8[((IOTA - 42) & 127)];
			double fTemp492 = fVec8[((IOTA - 41) & 127)];
			double fTemp493 = fVec8[((IOTA - 40) & 127)];
			double fTemp494 = fVec8[((IOTA - 39) & 127)];
			double fTemp495 = fVec8[((IOTA - 38) & 127)];
			double fTemp496 = fVec8[((IOTA - 29) & 127)];
			double fTemp497 = fVec8[((IOTA - 30) & 127)];
			double fTemp498 = fVec8[((IOTA - 25) & 127)];
			double fTemp499 = fVec8[((IOTA - 27) & 127)];
			double fTemp500 = fVec8[((IOTA - 26) & 127)];
			double fTemp501 = fVec8[((IOTA - 23) & 127)];
			double fTemp502 = fVec8[((IOTA - 22) & 127)];
			double fTemp503 = fVec8[((IOTA - 21) & 127)];
			double fTemp504 = fVec8[((IOTA - 20) & 127)];
			double fTemp505 = fVec8[((IOTA - 19) & 127)];
			double fTemp506 = fVec8[((IOTA - 18) & 127)];
			double fTemp507 = fVec8[((IOTA - 16) & 127)];
			double fTemp508 = fVec8[((IOTA - 17) & 127)];
			double fTemp509 = fVec8[((IOTA - 15) & 127)];
			double fTemp510 = fVec8[((IOTA - 14) & 127)];
			double fTemp511 = fVec3[((IOTA - 84) & 127)];
			double fTemp512 = fVec3[((IOTA - 101) & 127)];
			double fTemp513 = fVec3[((IOTA - 100) & 127)];
			double fTemp514 = fVec3[((IOTA - 119) & 127)];
			double fTemp515 = fVec3[((IOTA - 54) & 127)];
			double fTemp516 = fVec3[((IOTA - 49) & 127)];
			double fTemp517 = fVec3[((IOTA - 47) & 127)];
			double fTemp518 = fVec3[((IOTA - 48) & 127)];
			double fTemp519 = fVec3[((IOTA - 45) & 127)];
			double fTemp520 = fVec3[((IOTA - 51) & 127)];
			double fTemp521 = fVec3[((IOTA - 52) & 127)];
			double fTemp522 = fVec3[((IOTA - 42) & 127)];
			double fTemp523 = fVec4[((IOTA - 120) & 127)];
			double fTemp524 = fVec4[((IOTA - 121) & 127)];
			double fTemp525 = fVec4[((IOTA - 67) & 127)];
			double fTemp526 = fVec4[((IOTA - 66) & 127)];
			double fTemp527 = fVec4[((IOTA - 68) & 127)];
			double fTemp528 = fVec5[((IOTA - 20) & 127)];
			double fTemp529 = fVec5[((IOTA - 18) & 127)];
			double fTemp530 = fVec5[((IOTA - 17) & 127)];
			double fTemp531 = fVec5[((IOTA - 16) & 127)];
			double fTemp532 = fVec5[((IOTA - 15) & 127)];
			double fTemp533 = fVec5[((IOTA - 13) & 127)];
			double fTemp534 = fVec5[((IOTA - 10) & 127)];
			double fTemp535 = fVec5[((IOTA - 8) & 127)];
			double fTemp536 = fVec5[((IOTA - 6) & 127)];
			double fTemp537 = fVec5[((IOTA - 4) & 127)];
			double fTemp538 = fVec5[((IOTA - 2) & 127)];
			double fTemp539 = fVec5[((IOTA - 1) & 127)];
			double fTemp540 = fVec6[((IOTA - 78) & 127)];
			double fTemp541 = fVec7[((IOTA - 80) & 127)];
			double fTemp542 = fVec7[((IOTA - 79) & 127)];
			double fTemp543 = fVec6[((IOTA - 73) & 127)];
			double fTemp544 = fVec6[((IOTA - 72) & 127)];
			double fTemp545 = fVec6[((IOTA - 71) & 127)];
			double fTemp546 = fVec8[((IOTA - 80) & 127)];
			double fTemp547 = fVec8[((IOTA - 126) & 127)];
			double fTemp548 = fVec8[((IOTA - 123) & 127)];
			double fTemp549 = fVec8[((IOTA - 79) & 127)];
			double fTemp550 = fVec8[((IOTA - 78) & 127)];
			double fTemp551 = fVec8[((IOTA - 77) & 127)];
			double fTemp552 = fVec8[((IOTA - 48) & 127)];
			double fTemp553 = fVec8[((IOTA - 49) & 127)];
			double fTemp554 = fVec8[((IOTA - 46) & 127)];
			double fTemp555 = fVec8[((IOTA - 13) & 127)];
			double fTemp556 = fVec8[((IOTA - 12) & 127)];
			double fTemp557 = fVec8[((IOTA - 11) & 127)];
			double fTemp558 = fVec8[((IOTA - 10) & 127)];
			double fTemp559 = fVec8[((IOTA - 9) & 127)];
			double fTemp560 = fVec8[((IOTA - 8) & 127)];
			double fTemp561 = fVec8[((IOTA - 7) & 127)];
			double fTemp562 = fVec8[((IOTA - 6) & 127)];
			double fTemp563 = fVec8[((IOTA - 5) & 127)];
			double fTemp564 = fVec8[((IOTA - 4) & 127)];
			double fTemp565 = fVec8[((IOTA - 3) & 127)];
			double fTemp566 = fVec8[((IOTA - 1) & 127)];
			double fTemp567 = fVec5[((IOTA - 122) & 127)];
			double fTemp568 = fVec5[((IOTA - 30) & 127)];
			double fTemp569 = fVec5[((IOTA - 115) & 127)];
			double fTemp570 = fVec5[((IOTA - 86) & 127)];
			double fTemp571 = fVec7[((IOTA - 124) & 127)];
			double fTemp572 = fVec7[((IOTA - 123) & 127)];
			double fTemp573 = fVec7[((IOTA - 126) & 127)];
			double fTemp574 = fVec6[((IOTA - 107) & 127)];
			double fTemp575 = fVec8[((IOTA - 117) & 127)];
			double fTemp576 = fVec8[((IOTA - 116) & 127)];
			double fTemp577 = fVec8[((IOTA - 50) & 127)];
			double fTemp578 = fVec8[((IOTA - 51) & 127)];
			double fTemp579 = fVec6[((IOTA - 22) & 127)];
			double fTemp580 = fVec6[((IOTA - 19) & 127)];
			double fTemp581 = fVec6[((IOTA - 20) & 127)];
			double fTemp582 = fVec6[((IOTA - 21) & 127)];
			double fTemp583 = fVec6[((IOTA - 17) & 127)];
			double fTemp584 = fVec6[((IOTA - 18) & 127)];
			double fTemp585 = fVec6[((IOTA - 16) & 127)];
			double fTemp586 = fVec6[((IOTA - 15) & 127)];
			double fTemp587 = fVec6[((IOTA - 13) & 127)];
			double fTemp588 = fVec6[((IOTA - 14) & 127)];
			double fTemp589 = fVec6[((IOTA - 11) & 127)];
			double fTemp590 = fVec6[((IOTA - 12) & 127)];
			double fTemp591 = fVec6[((IOTA - 9) & 127)];
			double fTemp592 = fVec6[((IOTA - 7) & 127)];
			double fTemp593 = fVec6[((IOTA - 5) & 127)];
			double fTemp594 = fVec6[((IOTA - 4) & 127)];
			double fTemp595 = fVec6[((IOTA - 3) & 127)];
			double fTemp596 = fVec6[((IOTA - 1) & 127)];
			double fTemp597 = fVec8[((IOTA - 107) & 127)];
			double fTemp598 = fVec8[((IOTA - 106) & 127)];
			double fTemp599 = fVec8[((IOTA - 115) & 127)];
			double fTemp600 = fVec8[((IOTA - 58) & 127)];
			double fTemp601 = fVec8[((IOTA - 57) & 127)];
			double fTemp602 = fVec8[((IOTA - 37) & 127)];
			double fTemp603 = fVec8[((IOTA - 35) & 127)];
			double fTemp604 = fVec8[((IOTA - 36) & 127)];
			double fTemp605 = fVec8[((IOTA - 34) & 127)];
			double fTemp606 = fVec8[((IOTA - 33) & 127)];
			double fTemp607 = fVec8[((IOTA - 32) & 127)];
			double fTemp608 = fVec8[((IOTA - 31) & 127)];
			double fTemp609 = fVec2[((IOTA - 39) & 127)];
			double fTemp610 = fVec2[((IOTA - 37) & 127)];
			double fTemp611 = fVec2[((IOTA - 36) & 127)];
			double fTemp612 = fVec2[((IOTA - 32) & 127)];
			double fTemp613 = fVec2[((IOTA - 33) & 127)];
			double fTemp614 = fVec2[((IOTA - 29) & 127)];
			double fTemp615 = fVec2[((IOTA - 30) & 127)];
			double fTemp616 = fVec3[((IOTA - 65) & 127)];
			double fTemp617 = fVec2[((IOTA - 28) & 127)];
			double fTemp618 = fVec1[((IOTA - 112) & 127)];
			double fTemp619 = fVec0[((IOTA - 22) & 127)];
			double fTemp620 = fVec0[((IOTA - 125) & 127)];
			double fTemp621 = fVec0[((IOTA - 116) & 127)];
			double fTemp622 = fVec0[((IOTA - 124) & 127)];
			double fTemp623 = fVec0[((IOTA - 123) & 127)];
			double fTemp624 = fVec0[((IOTA - 111) & 127)];
			double fTemp625 = fVec4[((IOTA - 109) & 127)];
			double fTemp626 = fVec1[((IOTA - 78) & 127)];
			double fTemp627 = fVec0[((IOTA - 112) & 127)];
			double fTemp628 = fVec0[((IOTA - 101) & 127)];
			double fTemp629 = fVec0[((IOTA - 100) & 127)];
			double fTemp630 = fVec0[((IOTA - 115) & 127)];
			double fTemp631 = fVec0[((IOTA - 114) & 127)];
			double fTemp632 = fVec0[((IOTA - 99) & 127)];
			double fTemp633 = fVec0[((IOTA - 98) & 127)];
			double fTemp634 = fVec0[((IOTA - 85) & 127)];
			double fTemp635 = fVec0[((IOTA - 73) & 127)];
			double fTemp636 = fVec0[((IOTA - 63) & 127)];
			double fTemp637 = fVec0[((IOTA - 62) & 127)];
			double fTemp638 = fVec0[((IOTA - 59) & 127)];
			double fTemp639 = fVec0[((IOTA - 61) & 127)];
			double fTemp640 = fVec0[((IOTA - 54) & 127)];
			double fTemp641 = fVec0[((IOTA - 55) & 127)];
			double fTemp642 = fVec0[((IOTA - 56) & 127)];
			double fTemp643 = fVec0[((IOTA - 58) & 127)];
			double fTemp644 = fVec0[((IOTA - 57) & 127)];
			double fTemp645 = fVec0[((IOTA - 60) & 127)];
			double fTemp646 = fVec0[((IOTA - 52) & 127)];
			double fTemp647 = fVec0[((IOTA - 51) & 127)];
			double fTemp648 = fVec0[((IOTA - 50) & 127)];
			double fTemp649 = fVec0[((IOTA - 39) & 127)];
			double fTemp650 = fVec0[((IOTA - 40) & 127)];
			double fTemp651 = fVec0[((IOTA - 41) & 127)];
			double fTemp652 = fVec0[((IOTA - 37) & 127)];
			double fTemp653 = fVec0[((IOTA - 36) & 127)];
			double fTemp654 = fVec1[((IOTA - 123) & 127)];
			double fTemp655 = fVec1[((IOTA - 115) & 127)];
			double fTemp656 = fVec1[((IOTA - 114) & 127)];
			double fTemp657 = fVec1[((IOTA - 113) & 127)];
			double fTemp658 = fVec1[((IOTA - 95) & 127)];
			double fTemp659 = fVec1[((IOTA - 109) & 127)];
			double fTemp660 = fVec1[((IOTA - 80) & 127)];
			double fTemp661 = fVec1[((IOTA - 81) & 127)];
			double fTemp662 = fVec1[((IOTA - 79) & 127)];
			double fTemp663 = fVec1[((IOTA - 75) & 127)];
			double fTemp664 = fVec1[((IOTA - 74) & 127)];
			double fTemp665 = fVec1[((IOTA - 73) & 127)];
			double fTemp666 = fVec1[((IOTA - 69) & 127)];
			double fTemp667 = fVec1[((IOTA - 60) & 127)];
			double fTemp668 = fVec1[((IOTA - 49) & 127)];
			double fTemp669 = fVec1[((IOTA - 48) & 127)];
			double fTemp670 = fVec1[((IOTA - 47) & 127)];
			double fTemp671 = fVec1[((IOTA - 46) & 127)];
			double fTemp672 = fVec1[((IOTA - 45) & 127)];
			double fTemp673 = fVec1[((IOTA - 43) & 127)];
			double fTemp674 = fVec1[((IOTA - 40) & 127)];
			double fTemp675 = fVec1[((IOTA - 42) & 127)];
			double fTemp676 = fVec1[((IOTA - 39) & 127)];
			double fTemp677 = fVec1[((IOTA - 34) & 127)];
			double fTemp678 = fVec1[((IOTA - 33) & 127)];
			double fTemp679 = fVec1[((IOTA - 30) & 127)];
			double fTemp680 = fVec1[((IOTA - 27) & 127)];
			double fTemp681 = fVec1[((IOTA - 28) & 127)];
			double fTemp682 = fVec0[((IOTA - 119) & 127)];
			double fTemp683 = fVec0[((IOTA - 97) & 127)];
			double fTemp684 = fVec0[((IOTA - 93) & 127)];
			double fTemp685 = fVec0[((IOTA - 90) & 127)];
			double fTemp686 = fVec0[((IOTA - 92) & 127)];
			double fTemp687 = fVec0[((IOTA - 78) & 127)];
			double fTemp688 = fVec0[((IOTA - 83) & 127)];
			double fTemp689 = fVec0[((IOTA - 82) & 127)];
			double fTemp690 = fVec0[((IOTA - 89) & 127)];
			double fTemp691 = fVec0[((IOTA - 88) & 127)];
			double fTemp692 = fVec0[((IOTA - 44) & 127)];
			double fTemp693 = fVec0[((IOTA - 43) & 127)];
			double fTemp694 = fVec0[((IOTA - 31) & 127)];
			double fTemp695 = fVec0[((IOTA - 21) & 127)];
			double fTemp696 = fVec0[((IOTA - 17) & 127)];
			double fTemp697 = fVec0[((IOTA - 16) & 127)];
			double fTemp698 = fVec0[((IOTA - 15) & 127)];
			double fTemp699 = fVec0[((IOTA - 14) & 127)];
			double fTemp700 = fVec0[((IOTA - 13) & 127)];
			double fTemp701 = fVec0[((IOTA - 12) & 127)];
			double fTemp702 = fVec0[((IOTA - 11) & 127)];
			double fTemp703 = fVec0[((IOTA - 9) & 127)];
			double fTemp704 = fVec0[((IOTA - 7) & 127)];
			double fTemp705 = fVec0[((IOTA - 5) & 127)];
			double fTemp706 = fVec0[((IOTA - 3) & 127)];
			double fTemp707 = fVec0[((IOTA - 1) & 127)];
			double fTemp708 = fVec1[((IOTA - 118) & 127)];
			double fTemp709 = fVec1[((IOTA - 83) & 127)];
			double fTemp710 = fVec1[((IOTA - 88) & 127)];
			double fTemp711 = fVec1[((IOTA - 84) & 127)];
			double fTemp712 = fVec1[((IOTA - 82) & 127)];
			double fTemp713 = fVec2[((IOTA - 117) & 127)];
			double fTemp714 = fVec2[((IOTA - 116) & 127)];
			double fTemp715 = fVec2[((IOTA - 95) & 127)];
			double fTemp716 = fVec2[((IOTA - 23) & 127)];
			double fTemp717 = fVec2[((IOTA - 27) & 127)];
			double fTemp718 = fVec3[((IOTA - 117) & 127)];
			double fTemp719 = fVec3[((IOTA - 118) & 127)];
			double fTemp720 = fVec2[((IOTA - 89) & 127)];
			double fTemp721 = fVec1[((IOTA - 24) & 127)];
			double fTemp722 = fVec1[((IOTA - 23) & 127)];
			double fTemp723 = fVec1[((IOTA - 22) & 127)];
			double fTemp724 = fVec1[((IOTA - 21) & 127)];
			double fTemp725 = fVec1[((IOTA - 20) & 127)];
			double fTemp726 = fVec1[((IOTA - 17) & 127)];
			double fTemp727 = fVec1[((IOTA - 16) & 127)];
			double fTemp728 = fVec1[((IOTA - 14) & 127)];
			double fTemp729 = fVec1[((IOTA - 13) & 127)];
			double fTemp730 = fVec1[((IOTA - 8) & 127)];
			double fTemp731 = fVec1[((IOTA - 1) & 127)];
			double fTemp732 = fVec1[((IOTA - 5) & 127)];
			double fTemp733 = fVec1[((IOTA - 4) & 127)];
			double fTemp734 = fVec2[((IOTA - 124) & 127)];
			double fTemp735 = fVec2[((IOTA - 122) & 127)];
			double fTemp736 = fVec2[((IOTA - 120) & 127)];
			double fTemp737 = fVec2[((IOTA - 123) & 127)];
			double fTemp738 = fVec2[((IOTA - 113) & 127)];
			double fTemp739 = fVec2[((IOTA - 114) & 127)];
			double fTemp740 = fVec2[((IOTA - 103) & 127)];
			double fTemp741 = fVec2[((IOTA - 111) & 127)];
			double fTemp742 = fVec2[((IOTA - 112) & 127)];
			double fTemp743 = fVec2[((IOTA - 121) & 127)];
			double fTemp744 = fVec2[((IOTA - 85) & 127)];
			double fTemp745 = fVec2[((IOTA - 81) & 127)];
			double fTemp746 = fVec2[((IOTA - 78) & 127)];
			double fTemp747 = fVec2[((IOTA - 80) & 127)];
			double fTemp748 = fVec2[((IOTA - 86) & 127)];
			double fTemp749 = fVec2[((IOTA - 77) & 127)];
			double fTemp750 = fVec2[((IOTA - 76) & 127)];
			double fTemp751 = fVec2[((IOTA - 73) & 127)];
			double fTemp752 = fVec2[((IOTA - 74) & 127)];
			double fTemp753 = fVec2[((IOTA - 68) & 127)];
			double fTemp754 = fVec2[((IOTA - 67) & 127)];
			double fTemp755 = fVec2[((IOTA - 62) & 127)];
			double fTemp756 = fVec2[((IOTA - 59) & 127)];
			double fTemp757 = fVec2[((IOTA - 61) & 127)];
			double fTemp758 = fVec2[((IOTA - 58) & 127)];
			double fTemp759 = fVec2[((IOTA - 69) & 127)];
			double fTemp760 = fVec2[((IOTA - 57) & 127)];
			double fTemp761 = fVec2[((IOTA - 54) & 127)];
			double fTemp762 = fVec2[((IOTA - 55) & 127)];
			double fTemp763 = fVec2[((IOTA - 56) & 127)];
			double fTemp764 = fVec2[((IOTA - 51) & 127)];
			double fTemp765 = fVec2[((IOTA - 50) & 127)];
			double fTemp766 = fVec2[((IOTA - 53) & 127)];
			double fTemp767 = fVec2[((IOTA - 47) & 127)];
			double fTemp768 = fVec2[((IOTA - 49) & 127)];
			double fTemp769 = fVec2[((IOTA - 48) & 127)];
			double fTemp770 = fVec2[((IOTA - 46) & 127)];
			double fTemp771 = fVec2[((IOTA - 45) & 127)];
			double fTemp772 = fVec2[((IOTA - 44) & 127)];
			double fTemp773 = fVec2[((IOTA - 43) & 127)];
			double fTemp774 = fVec2[((IOTA - 42) & 127)];
			double fTemp775 = fVec3[((IOTA - 122) & 127)];
			double fTemp776 = fVec3[((IOTA - 121) & 127)];
			double fTemp777 = fVec3[((IOTA - 112) & 127)];
			double fTemp778 = fVec3[((IOTA - 111) & 127)];
			double fTemp779 = fVec3[((IOTA - 120) & 127)];
			double fTemp780 = fVec3[((IOTA - 79) & 127)];
			double fTemp781 = fVec3[((IOTA - 81) & 127)];
			double fTemp782 = fVec3[((IOTA - 82) & 127)];
			double fTemp783 = fVec3[((IOTA - 80) & 127)];
			double fTemp784 = fVec3[((IOTA - 70) & 127)];
			double fTemp785 = fVec3[((IOTA - 69) & 127)];
			double fTemp786 = fVec3[((IOTA - 63) & 127)];
			double fTemp787 = fVec3[((IOTA - 61) & 127)];
			double fTemp788 = fVec3[((IOTA - 68) & 127)];
			double fTemp789 = fVec3[((IOTA - 62) & 127)];
			double fTemp790 = fVec3[((IOTA - 34) & 127)];
			double fTemp791 = fVec3[((IOTA - 30) & 127)];
			double fTemp792 = fVec3[((IOTA - 31) & 127)];
			double fTemp793 = fVec3[((IOTA - 27) & 127)];
			double fTemp794 = fVec3[((IOTA - 26) & 127)];
			double fTemp795 = fVec3[((IOTA - 25) & 127)];
			double fTemp796 = fVec3[((IOTA - 19) & 127)];
			double fTemp797 = fVec3[((IOTA - 18) & 127)];
			double fTemp798 = fVec3[((IOTA - 16) & 127)];
			double fTemp799 = fVec3[((IOTA - 15) & 127)];
			double fTemp800 = fVec3[((IOTA - 12) & 127)];
			double fTemp801 = fVec3[((IOTA - 11) & 127)];
			double fTemp802 = fVec3[((IOTA - 9) & 127)];
			double fTemp803 = fVec3[((IOTA - 7) & 127)];
			double fTemp804 = fVec3[((IOTA - 3) & 127)];
			double fTemp805 = fVec3[((IOTA - 37) & 127)];
			double fTemp806 = fVec4[((IOTA - 118) & 127)];
			double fTemp807 = fVec4[((IOTA - 115) & 127)];
			double fTemp808 = fVec4[((IOTA - 100) & 127)];
			double fTemp809 = fVec4[((IOTA - 99) & 127)];
			double fTemp810 = fVec4[((IOTA - 102) & 127)];
			double fTemp811 = fVec3[((IOTA - 59) & 127)];
			double fTemp812 = fVec4[((IOTA - 96) & 127)];
			double fTemp813 = fVec4[((IOTA - 88) & 127)];
			double fTemp814 = fVec4[((IOTA - 89) & 127)];
			double fTemp815 = fVec4[((IOTA - 85) & 127)];
			double fTemp816 = fVec4[((IOTA - 90) & 127)];
			double fTemp817 = fVec4[((IOTA - 76) & 127)];
			double fTemp818 = fVec4[((IOTA - 84) & 127)];
			double fTemp819 = fVec4[((IOTA - 82) & 127)];
			double fTemp820 = fVec4[((IOTA - 103) & 127)];
			double fTemp821 = fVec4[((IOTA - 64) & 127)];
			double fTemp822 = fVec4[((IOTA - 73) & 127)];
			double fTemp823 = fVec4[((IOTA - 63) & 127)];
			double fTemp824 = fVec4[((IOTA - 60) & 127)];
			double fTemp825 = fVec4[((IOTA - 47) & 127)];
			double fTemp826 = fVec4[((IOTA - 46) & 127)];
			double fTemp827 = fVec4[((IOTA - 43) & 127)];
			double fTemp828 = fVec4[((IOTA - 41) & 127)];
			double fTemp829 = fVec4[((IOTA - 34) & 127)];
			double fTemp830 = fVec4[((IOTA - 32) & 127)];
			double fTemp831 = fVec4[((IOTA - 33) & 127)];
			double fTemp832 = fVec4[((IOTA - 29) & 127)];
			double fTemp833 = fVec4[((IOTA - 27) & 127)];
			double fTemp834 = fVec0[((IOTA - 108) & 127)];
			double fTemp835 = fVec0[((IOTA - 107) & 127)];
			double fTemp836 = fVec0[((IOTA - 105) & 127)];
			double fTemp837 = fVec0[((IOTA - 102) & 127)];
			double fTemp838 = fVec0[((IOTA - 87) & 127)];
			double fTemp839 = fVec0[((IOTA - 80) & 127)];
			double fTemp840 = fVec0[((IOTA - 48) & 127)];
			double fTemp841 = fVec0[((IOTA - 47) & 127)];
			double fTemp842 = fVec0[((IOTA - 46) & 127)];
			double fTemp843 = fVec0[((IOTA - 35) & 127)];
			double fTemp844 = fVec0[((IOTA - 45) & 127)];
			double fTemp845 = fVec0[((IOTA - 32) & 127)];
			double fTemp846 = fVec1[((IOTA - 122) & 127)];
			double fTemp847 = fVec1[((IOTA - 96) & 127)];
			double fTemp848 = fVec1[((IOTA - 90) & 127)];
			double fTemp849 = fVec1[((IOTA - 89) & 127)];
			double fTemp850 = fVec1[((IOTA - 65) & 127)];
			double fTemp851 = fVec1[((IOTA - 87) & 127)];
			double fTemp852 = fVec1[((IOTA - 53) & 127)];
			double fTemp853 = fVec1[((IOTA - 37) & 127)];
			double fTemp854 = fVec1[((IOTA - 52) & 127)];
			double fTemp855 = fVec2[((IOTA - 17) & 127)];
			double fTemp856 = fVec2[((IOTA - 16) & 127)];
			double fTemp857 = fVec2[((IOTA - 15) & 127)];
			double fTemp858 = fVec2[((IOTA - 14) & 127)];
			double fTemp859 = fVec2[((IOTA - 13) & 127)];
			double fTemp860 = fVec2[((IOTA - 12) & 127)];
			double fTemp861 = fVec2[((IOTA - 11) & 127)];
			double fTemp862 = fVec2[((IOTA - 9) & 127)];
			double fTemp863 = fVec2[((IOTA - 7) & 127)];
			double fTemp864 = fVec2[((IOTA - 5) & 127)];
			double fTemp865 = fVec2[((IOTA - 4) & 127)];
			double fTemp866 = fVec2[((IOTA - 3) & 127)];
			double fTemp867 = fVec2[((IOTA - 1) & 127)];
			double fTemp868 = fVec4[((IOTA - 122) & 127)];
			double fTemp869 = fVec4[((IOTA - 91) & 127)];
			double fTemp870 = fVec4[((IOTA - 81) & 127)];
			double fTemp871 = fVec4[((IOTA - 80) & 127)];
			double fTemp872 = fVec4[((IOTA - 77) & 127)];
			double fTemp873 = fVec4[((IOTA - 79) & 127)];
			double fTemp874 = fVec4[((IOTA - 54) & 127)];
			double fTemp875 = fVec4[((IOTA - 51) & 127)];
			double fTemp876 = fVec4[((IOTA - 55) & 127)];
			double fTemp877 = fVec0[((IOTA - 118) & 127)];
			double fTemp878 = fVec0[((IOTA - 113) & 127)];
			double fTemp879 = fVec0[((IOTA - 109) & 127)];
			double fTemp880 = fVec0[((IOTA - 106) & 127)];
			double fTemp881 = fVec0[((IOTA - 84) & 127)];
			double fTemp882 = fVec0[((IOTA - 81) & 127)];
			double fTemp883 = fVec0[((IOTA - 25) & 127)];
			double fTemp884 = fVec1[((IOTA - 124) & 127)];
			double fTemp885 = fVec1[((IOTA - 120) & 127)];
			double fTemp886 = fVec1[((IOTA - 92) & 127)];
			double fTemp887 = fVec1[((IOTA - 91) & 127)];
			double fTemp888 = fVec2[((IOTA - 125) & 127)];
			double fTemp889 = fVec2[((IOTA - 115) & 127)];
			double fTemp890 = fVec1[((IOTA - 119) & 127)];
			double fTemp891 = fVec2[((IOTA - 96) & 127)];
			double fTemp892 = fVec6[((IOTA - 48) & 127)];
			double fTemp893 = fVec6[((IOTA - 47) & 127)];
			double fTemp894 = fVec6[((IOTA - 46) & 127)];
			double fTemp895 = fVec6[((IOTA - 45) & 127)];
			double fTemp896 = fVec6[((IOTA - 42) & 127)];
			double fTemp897 = fVec4[((IOTA - 5) & 127)];
			double fTemp898 = fVec4[((IOTA - 3) & 127)];
			double fTemp899 = fVec4[((IOTA - 23) & 127)];
			double fTemp900 = fVec4[((IOTA - 24) & 127)];
			double fTemp901 = fVec4[((IOTA - 22) & 127)];
			double fTemp902 = fVec4[((IOTA - 21) & 127)];
			double fTemp903 = fVec4[((IOTA - 20) & 127)];
			double fTemp904 = fVec4[((IOTA - 19) & 127)];
			double fTemp905 = fVec4[((IOTA - 14) & 127)];
			double fTemp906 = fVec4[((IOTA - 12) & 127)];
			double fTemp907 = fVec4[((IOTA - 11) & 127)];
			double fTemp908 = fVec4[((IOTA - 9) & 127)];
			double fTemp909 = fVec4[((IOTA - 7) & 127)];
			double fTemp910 = fVec5[((IOTA - 124) & 127)];
			double fTemp911 = fVec4[((IOTA - 28) & 127)];
			double fTemp912 = fVec5[((IOTA - 118) & 127)];
			double fTemp913 = fVec5[((IOTA - 114) & 127)];
			double fTemp914 = fVec5[((IOTA - 113) & 127)];
			double fTemp915 = fVec5[((IOTA - 111) & 127)];
			double fTemp916 = fVec5[((IOTA - 107) & 127)];
			double fTemp917 = fVec5[((IOTA - 106) & 127)];
			double fTemp918 = fVec5[((IOTA - 104) & 127)];
			double fTemp919 = fVec5[((IOTA - 103) & 127)];
			double fTemp920 = fVec5[((IOTA - 102) & 127)];
			double fTemp921 = fVec5[((IOTA - 100) & 127)];
			double fTemp922 = fVec5[((IOTA - 101) & 127)];
			double fTemp923 = fVec5[((IOTA - 98) & 127)];
			double fTemp924 = fVec5[((IOTA - 92) & 127)];
			double fTemp925 = fVec5[((IOTA - 93) & 127)];
			double fTemp926 = fVec5[((IOTA - 99) & 127)];
			double fTemp927 = fVec5[((IOTA - 97) & 127)];
			double fTemp928 = fVec5[((IOTA - 105) & 127)];
			double fTemp929 = fVec5[((IOTA - 109) & 127)];
			double fTemp930 = fVec5[((IOTA - 108) & 127)];
			double fTemp931 = fVec5[((IOTA - 88) & 127)];
			double fTemp932 = fVec5[((IOTA - 81) & 127)];
			double fTemp933 = fVec5[((IOTA - 80) & 127)];
			double fTemp934 = fVec5[((IOTA - 82) & 127)];
			double fTemp935 = fVec5[((IOTA - 76) & 127)];
			double fTemp936 = fVec5[((IOTA - 75) & 127)];
			double fTemp937 = fVec5[((IOTA - 79) & 127)];
			double fTemp938 = fVec5[((IOTA - 78) & 127)];
			double fTemp939 = fVec5[((IOTA - 77) & 127)];
			double fTemp940 = fVec5[((IOTA - 68) & 127)];
			double fTemp941 = fVec5[((IOTA - 74) & 127)];
			double fTemp942 = fVec5[((IOTA - 66) & 127)];
			double fTemp943 = fVec5[((IOTA - 61) & 127)];
			double fTemp944 = fVec5[((IOTA - 67) & 127)];
			double fTemp945 = fVec5[((IOTA - 73) & 127)];
			double fTemp946 = fVec5[((IOTA - 59) & 127)];
			double fTemp947 = fVec5[((IOTA - 58) & 127)];
			double fTemp948 = fVec5[((IOTA - 60) & 127)];
			double fTemp949 = fVec5[((IOTA - 41) & 127)];
			double fTemp950 = fVec5[((IOTA - 40) & 127)];
			double fTemp951 = fVec5[((IOTA - 47) & 127)];
			double fTemp952 = fVec5[((IOTA - 35) & 127)];
			double fTemp953 = fVec5[((IOTA - 32) & 127)];
			double fTemp954 = fVec5[((IOTA - 28) & 127)];
			double fTemp955 = fVec5[((IOTA - 27) & 127)];
			double fTemp956 = fVec5[((IOTA - 25) & 127)];
			double fTemp957 = fVec5[((IOTA - 24) & 127)];
			double fTemp958 = fVec5[((IOTA - 50) & 127)];
			double fTemp959 = fVec5[((IOTA - 46) & 127)];
			double fTemp960 = fVec7[((IOTA - 115) & 127)];
			double fTemp961 = fVec7[((IOTA - 114) & 127)];
			double fTemp962 = fVec7[((IOTA - 113) & 127)];
			double fTemp963 = fVec7[((IOTA - 110) & 127)];
			double fTemp964 = fVec7[((IOTA - 106) & 127)];
			double fTemp965 = fVec7[((IOTA - 103) & 127)];
			double fTemp966 = fVec7[((IOTA - 105) & 127)];
			double fTemp967 = fVec7[((IOTA - 104) & 127)];
			double fTemp968 = fVec7[((IOTA - 101) & 127)];
			double fTemp969 = fVec7[((IOTA - 99) & 127)];
			double fTemp970 = fVec7[((IOTA - 96) & 127)];
			double fTemp971 = fVec7[((IOTA - 95) & 127)];
			double fTemp972 = fVec7[((IOTA - 91) & 127)];
			double fTemp973 = fVec7[((IOTA - 102) & 127)];
			double fTemp974 = fVec7[((IOTA - 107) & 127)];
			double fTemp975 = fVec7[((IOTA - 111) & 127)];
			double fTemp976 = fVec7[((IOTA - 100) & 127)];
			double fTemp977 = fVec7[((IOTA - 74) & 127)];
			double fTemp978 = fVec7[((IOTA - 73) & 127)];
			double fTemp979 = fVec7[((IOTA - 76) & 127)];
			double fTemp980 = fVec7[((IOTA - 71) & 127)];
			double fTemp981 = fVec7[((IOTA - 70) & 127)];
			double fTemp982 = fVec7[((IOTA - 69) & 127)];
			double fTemp983 = fVec7[((IOTA - 72) & 127)];
			double fTemp984 = fVec7[((IOTA - 65) & 127)];
			double fTemp985 = fVec7[((IOTA - 66) & 127)];
			double fTemp986 = fVec7[((IOTA - 64) & 127)];
			double fTemp987 = fVec7[((IOTA - 61) & 127)];
			double fTemp988 = fVec7[((IOTA - 68) & 127)];
			double fTemp989 = fVec7[((IOTA - 67) & 127)];
			double fTemp990 = fVec7[((IOTA - 57) & 127)];
			double fTemp991 = fVec7[((IOTA - 55) & 127)];
			double fTemp992 = fVec7[((IOTA - 56) & 127)];
			double fTemp993 = fVec7[((IOTA - 49) & 127)];
			double fTemp994 = fVec7[((IOTA - 43) & 127)];
			double fTemp995 = fVec7[((IOTA - 38) & 127)];
			double fTemp996 = fVec7[((IOTA - 37) & 127)];
			double fTemp997 = fVec7[((IOTA - 34) & 127)];
			double fTemp998 = fVec7[((IOTA - 31) & 127)];
			double fTemp999 = fVec7[((IOTA - 30) & 127)];
			double fTemp1000 = fVec7[((IOTA - 27) & 127)];
			double fTemp1001 = fVec7[((IOTA - 25) & 127)];
			double fTemp1002 = fVec7[((IOTA - 23) & 127)];
			double fTemp1003 = fVec7[((IOTA - 22) & 127)];
			double fTemp1004 = fVec7[((IOTA - 19) & 127)];
			double fTemp1005 = fVec7[((IOTA - 18) & 127)];
			double fTemp1006 = fVec7[((IOTA - 16) & 127)];
			double fTemp1007 = fVec7[((IOTA - 15) & 127)];
			double fTemp1008 = fVec7[((IOTA - 12) & 127)];
			double fTemp1009 = fVec7[((IOTA - 11) & 127)];
			double fTemp1010 = fVec7[((IOTA - 10) & 127)];
			double fTemp1011 = fVec7[((IOTA - 9) & 127)];
			double fTemp1012 = fVec7[((IOTA - 7) & 127)];
			double fTemp1013 = fVec7[((IOTA - 6) & 127)];
			double fTemp1014 = fVec7[((IOTA - 3) & 127)];
			double fTemp1015 = fVec6[((IOTA - 123) & 127)];
			double fTemp1016 = fVec6[((IOTA - 122) & 127)];
			double fTemp1017 = fVec6[((IOTA - 121) & 127)];
			double fTemp1018 = fVec6[((IOTA - 120) & 127)];
			double fTemp1019 = fVec6[((IOTA - 116) & 127)];
			double fTemp1020 = fVec6[((IOTA - 117) & 127)];
			double fTemp1021 = fVec6[((IOTA - 112) & 127)];
			double fTemp1022 = fVec6[((IOTA - 113) & 127)];
			double fTemp1023 = fVec6[((IOTA - 111) & 127)];
			double fTemp1024 = fVec6[((IOTA - 124) & 127)];
			double fTemp1025 = fVec6[((IOTA - 119) & 127)];
			double fTemp1026 = fVec6[((IOTA - 115) & 127)];
			double fTemp1027 = fVec6[((IOTA - 109) & 127)];
			double fTemp1028 = fVec6[((IOTA - 108) & 127)];
			double fTemp1029 = fVec6[((IOTA - 118) & 127)];
			double fTemp1030 = fVec6[((IOTA - 110) & 127)];
			double fTemp1031 = fVec6[((IOTA - 106) & 127)];
			double fTemp1032 = fVec6[((IOTA - 125) & 127)];
			double fTemp1033 = fVec6[((IOTA - 81) & 127)];
			double fTemp1034 = fVec6[((IOTA - 91) & 127)];
			double fTemp1035 = fVec6[((IOTA - 76) & 127)];
			double fTemp1036 = fVec6[((IOTA - 75) & 127)];
			double fTemp1037 = fVec6[((IOTA - 67) & 127)];
			double fTemp1038 = fVec6[((IOTA - 64) & 127)];
			double fTemp1039 = fVec6[((IOTA - 63) & 127)];
			double fTemp1040 = fVec6[((IOTA - 61) & 127)];
			double fTemp1041 = fVec6[((IOTA - 56) & 127)];
			double fTemp1042 = fVec6[((IOTA - 62) & 127)];
			double fTemp1043 = fVec6[((IOTA - 55) & 127)];
			double fTemp1044 = fVec6[((IOTA - 49) & 127)];
			double fTemp1045 = fVec6[((IOTA - 50) & 127)];
			double fTemp1046 = fVec6[((IOTA - 44) & 127)];
			double fTemp1047 = fVec6[((IOTA - 51) & 127)];
			double fTemp1048 = fVec6[((IOTA - 54) & 127)];
			double fTemp1049 = fVec6[((IOTA - 53) & 127)];
			double fTemp1050 = fVec6[((IOTA - 52) & 127)];
			double fTemp1051 = fVec8[((IOTA - 92) & 127)];
			double fTemp1052 = fVec8[((IOTA - 91) & 127)];
			double fTemp1053 = fVec8[((IOTA - 84) & 127)];
			double fTemp1054 = fVec8[((IOTA - 97) & 127)];
			double fTemp1055 = fVec8[((IOTA - 28) & 127)];
			double fTemp1056 = fVec8[((IOTA - 24) & 127)];
			double fTemp1057 = fVec3[((IOTA - 57) & 127)];
			double fTemp1058 = fVec3[((IOTA - 58) & 127)];
			double fTemp1059 = fVec3[((IOTA - 56) & 127)];
			double fTemp1060 = fVec3[((IOTA - 50) & 127)];
			double fTemp1061 = fVec3[((IOTA - 53) & 127)];
			double fTemp1062 = fVec3[((IOTA - 55) & 127)];
			double fTemp1063 = fVec3[((IOTA - 46) & 127)];
			double fTemp1064 = fVec3[((IOTA - 44) & 127)];
			double fTemp1065 = fVec3[((IOTA - 43) & 127)];
			double fTemp1066 = fVec3[((IOTA - 41) & 127)];
			double fTemp1067 = fVec4[((IOTA - 72) & 127)];
			double fTemp1068 = fVec4[((IOTA - 71) & 127)];
			double fTemp1069 = fVec4[((IOTA - 70) & 127)];
			double fTemp1070 = fVec4[((IOTA - 65) & 127)];
			double fTemp1071 = fVec4[((IOTA - 69) & 127)];
			double fTemp1072 = fVec5[((IOTA - 72) & 127)];
			double fTemp1073 = fVec5[((IOTA - 71) & 127)];
			double fTemp1074 = fVec5[((IOTA - 70) & 127)];
			double fTemp1075 = fVec5[((IOTA - 69) & 127)];
			double fTemp1076 = fVec5[((IOTA - 21) & 127)];
			double fTemp1077 = fVec5[((IOTA - 19) & 127)];
			double fTemp1078 = fVec5[((IOTA - 14) & 127)];
			double fTemp1079 = fVec5[((IOTA - 12) & 127)];
			double fTemp1080 = fVec5[((IOTA - 11) & 127)];
			double fTemp1081 = fVec5[((IOTA - 9) & 127)];
			double fTemp1082 = fVec5[((IOTA - 7) & 127)];
			double fTemp1083 = fVec5[((IOTA - 5) & 127)];
			double fTemp1084 = fVec5[((IOTA - 3) & 127)];
			double fTemp1085 = fVec7[((IOTA - 81) & 127)];
			double fTemp1086 = fVec7[((IOTA - 82) & 127)];
			double fTemp1087 = fVec6[((IOTA - 70) & 127)];
			double fTemp1088 = fVec6[((IOTA - 68) & 127)];
			double fTemp1089 = fVec8[((IOTA - 81) & 127)];
			double fTemp1090 = fVec8[((IOTA - 47) & 127)];
			double fTemp1091 = fVec6[((IOTA - 69) & 127)];
			double fTemp1092 = fVec8[((IOTA - 2) & 127)];
			double fTemp1093 = fVec5[((IOTA - 110) & 127)];
			double fTemp1094 = fVec5[((IOTA - 112) & 127)];
			double fTemp1095 = fVec5[((IOTA - 89) & 127)];
			double fTemp1096 = fVec5[((IOTA - 31) & 127)];
			double fTemp1097 = fVec4[((IOTA - 53) & 127)];
			double fTemp1098 = fVec7[((IOTA - 112) & 127)];
			double fTemp1099 = fVec7[((IOTA - 109) & 127)];
			double fTemp1100 = fVec7[((IOTA - 108) & 127)];
			double fTemp1101 = fVec6[((IOTA - 126) & 127)];
			double fTemp1102 = fVec6[((IOTA - 43) & 127)];
			double fTemp1103 = fVec6[((IOTA - 60) & 127)];
			double fTemp1104 = fVec6[((IOTA - 59) & 127)];
			double fTemp1105 = fVec6[((IOTA - 58) & 127)];
			double fTemp1106 = fVec6[((IOTA - 57) & 127)];
			double fTemp1107 = fVec8[((IOTA - 88) & 127)];
			double fTemp1108 = fVec8[((IOTA - 89) & 127)];
			double fTemp1109 = fVec6[((IOTA - 41) & 127)];
			double fTemp1110 = fVec6[((IOTA - 40) & 127)];
			double fTemp1111 = fVec6[((IOTA - 39) & 127)];
			double fTemp1112 = fVec6[((IOTA - 37) & 127)];
			double fTemp1113 = fVec6[((IOTA - 36) & 127)];
			double fTemp1114 = fVec6[((IOTA - 38) & 127)];
			double fTemp1115 = fVec6[((IOTA - 32) & 127)];
			double fTemp1116 = fVec6[((IOTA - 31) & 127)];
			double fTemp1117 = fVec6[((IOTA - 30) & 127)];
			double fTemp1118 = fVec6[((IOTA - 29) & 127)];
			double fTemp1119 = fVec6[((IOTA - 28) & 127)];
			double fTemp1120 = fVec6[((IOTA - 27) & 127)];
			double fTemp1121 = fVec6[((IOTA - 33) & 127)];
			double fTemp1122 = fVec6[((IOTA - 34) & 127)];
			double fTemp1123 = fVec6[((IOTA - 35) & 127)];
			double fTemp1124 = fVec6[((IOTA - 25) & 127)];
			double fTemp1125 = fVec6[((IOTA - 24) & 127)];
			double fTemp1126 = fVec6[((IOTA - 23) & 127)];
			double fTemp1127 = fVec6[((IOTA - 26) & 127)];
			double fTemp1128 = fVec6[((IOTA - 10) & 127)];
			double fTemp1129 = fVec6[((IOTA - 8) & 127)];
			double fTemp1130 = fVec6[((IOTA - 6) & 127)];
			double fTemp1131 = fVec6[((IOTA - 2) & 127)];
			double fTemp1132 = fVec6[((IOTA - 66) & 127)];
			double fTemp1133 = fVec6[((IOTA - 65) & 127)];
			double fTemp1134 = fVec6[((IOTA - 74) & 127)];
			double fTemp1135 = fVec2[((IOTA - 41) & 127)];
			double fTemp1136 = fVec2[((IOTA - 40) & 127)];
			double fTemp1137 = fVec2[((IOTA - 34) & 127)];
			double fTemp1138 = fVec2[((IOTA - 38) & 127)];
			double fTemp1139 = fVec2[((IOTA - 35) & 127)];
			double fTemp1140 = fVec2[((IOTA - 31) & 127)];
			double fTemp1141 = fVec3[((IOTA - 66) & 127)];
			double fTemp1142 = fVec0[((IOTA - 49) & 127)];
			double fTemp1143 = (fRec1[0] * (((5.611290000000001e-05 * fTemp1) + ((0.000144938 * fTemp2) + ((0.00098582100000000001 * fTemp3) + ((0.0010638000000000002 * fTemp4) + ((0.0018535400000000001 * fTemp5) + ((0.0020713300000000001 * fTemp6) + ((0.00287405 * fTemp7) + ((0.00066612700000000008 * fTemp8) + ((0.00021471700000000001 * fTemp9) + ((0.0083305600000000007 * fTemp10) + ((2.773e-06 * fTemp12) + ((2.8457499999999998e-06 * fTemp13) + ((2.4558300000000005e-05 * fTemp14) + ((5.3135e-06 * fTemp15) + ((0.00024796200000000001 * fTemp16) + ((0.00014221200000000001 * fTemp17) + ((0.00028535700000000003 * fTemp18) + ((0.00070562000000000001 * fTemp19) + ((0.0005151 * fTemp20) + ((0.00124614 * fTemp21) + ((0.00032077100000000002 * fTemp22) + ((9.0654800000000018e-05 * fTemp23) + ((0.0022713 * fTemp24) + ((6.2936500000000003e-05 * fTemp25) + ((2.0687900000000002e-05 * fTemp26) + ((0.00221192 * fTemp27) + ((0.00074920100000000001 * fTemp28) + ((0.00066226100000000003 * fTemp29) + ((0.0036181899999999999 * fTemp30) + ((3.8066800000000006e-05 * fTemp31) + ((0.0022166500000000001 * fTemp32) + ((0.0021549399999999997 * fTemp33) + ((0.0013761300000000001 * fTemp34) + ((0.023527200000000002 * fTemp35) + ((0.0021948500000000004 * fTemp36) + ((0.00051807200000000006 * fTemp37) + ((0.024944000000000001 * fTemp38) + ((0.0035849099999999997 * fTemp39) + ((1.06705e-06 * fTemp40) + ((3.8319600000000005e-05 * fTemp41) + ((0.0031629900000000001 * fTemp42) + ((0.0011606799999999999 * fTemp43) + ((0.00078389600000000007 * fTemp44) + ((3.61728e-05 * fTemp45) + ((7.2644700000000013e-05 * fTemp46) + ((1.0278999999999999e-06 * fTemp47) + ((0.00053547700000000002 * fTemp48) + ((0.00078204200000000005 * fTemp49) + ((0.00089492 * fTemp50) + ((0.0054627 * fTemp51) + ((0.0053478500000000003 * fTemp52) + ((0.0036506200000000003 * fTemp53) + ((0.0039108500000000004 * fTemp54) + ((0.00124406 * fTemp55) + ((0.041771900000000008 * fTemp56) + ((0.055177400000000001 * fTemp57) + ((0.0108086 * fTemp58) + ((0.0026607500000000004 * fTemp59) + ((0.00066903899999999996 * fTemp60) + ((0.0013556799999999999 * fTemp61) + ((0.00012807300000000001 * fTemp62) + ((0.000378556 * fTemp63) + ((4.9495899999999995e-07 * fTemp64) + ((0.00025383300000000002 * fTemp65) + ((0.00035608600000000003 * fTemp66) + ((0.00034403800000000002 * fTemp67) + ((0.00071143700000000009 * fTemp68) + ((0.0014366699999999999 * fTemp69) + ((0.0045955500000000003 * fTemp70) + ((0.00058266999999999998 * fTemp72) + ((0.00082486899999999999 * fTemp73) + ((0.0011506799999999998 * fTemp74) + ((0.00010084700000000001 * fTemp75) + ((3.5632800000000002e-05 * fTemp76) + ((0.00034166200000000001 * fTemp77) + ((3.64663e-05 * fTemp78) + ((0.035996800000000002 * fTemp79) + ((2.0105900000000003e-05 * fTemp81) + ((0.0124978 * fTemp82) + ((0.00053009599999999997 * fTemp83) + ((0.0011330000000000001 * fTemp84) + ((0.0016271899999999999 * fTemp85) + ((0.00069229100000000004 * fTemp86) + ((0.00050674400000000005 * fTemp87) + ((0.00049718700000000006 * fTemp88) + ((0.023463500000000002 * fTemp89) + ((0.052758300000000001 * fTemp90) + ((0.00038071900000000001 * fTemp91) + ((0.0094331200000000014 * fTemp92) + ((0.011300600000000001 * fTemp93) + ((0.0071840999999999997 * fTemp94) + ((0.00027813700000000002 * fTemp95) + ((0.00027818999999999999 * fTemp96) + ((0.00022609 * fTemp97) + ((0.00033955200000000002 * fTemp98) + ((2.4972599999999999e-05 * fTemp99) + ((5.2352700000000004e-05 * fTemp100) + ((4.2462999999999995e-06 * fTemp11) + ((0.00016021500000000001 * fTemp101) + ((0.000240886 * fTemp102) + ((3.0215200000000005e-05 * fTemp103) + ((0.000204718 * fTemp104) + ((0.00033797299999999998 * fTemp105) + ((0.00013590400000000001 * fTemp106) + ((6.0929900000000008e-05 * fTemp107) + ((0.000208667 * fTemp108) + ((0.00030785900000000001 * fTemp109) + ((0.00026147399999999998 * fTemp110) + ((0.00048677700000000003 * fTemp111) + ((0.0012107800000000001 * fTemp112) + ((0.00045785700000000004 * fTemp113) + ((4.86084e-05 * fTemp114) + ((8.9630500000000016e-08 * fTemp115) + ((0.00056137899999999998 * fTemp116) + ((0.00076490500000000008 * fTemp117) + ((0.00015598000000000001 * fTemp118) + ((0.00076870700000000009 * fTemp119) + ((0.0014322200000000001 * fTemp120) + ((0.00064690400000000001 * fTemp121) + ((0.00046067700000000005 * fTemp122) + ((0.00271718 * fTemp123) + ((6.2586200000000001e-05 * fTemp124) + ((0.00117567 * fTemp125) + ((0.0012521400000000001 * fTemp126) + ((0.00055114699999999999 * fTemp127) + ((0.0016155 * fTemp128) + ((0.0019438999999999999 * fTemp129) + ((0.00122438 * fTemp130) + ((0.0021511500000000001 * fTemp131) + ((0.00110439 * fTemp132) + ((3.43292e-08 * fTemp133) + ((3.3588899999999999e-06 * fTemp134) + ((3.9804799999999997e-06 * fTemp135) + ((5.0789799999999999e-05 * fTemp136) + ((3.2623300000000003e-05 * fTemp137) + ((0.00026027500000000002 * fTemp138) + ((0.00040571999999999998 * fTemp139) + ((0.00020187200000000002 * fTemp140) + ((0.00084254300000000007 * fTemp141) + ((0.00156339 * fTemp142) + ((0.0034697200000000004 * fTemp143) + ((0.0030116200000000004 * fTemp144) + ((0.00075465500000000002 * fTemp145) + ((0.00068181600000000002 * fTemp146) + ((0.00071934100000000008 * fTemp147) + ((0.00040184499999999996 * fTemp148) + ((0.00055143000000000011 * fTemp149) + ((0.0016723300000000001 * fTemp150) + ((0.0017063 * fTemp151) + ((0.00020361699999999999 * fTemp152) + ((0.00030080800000000001 * fTemp153) + ((0.0026214800000000003 * fTemp154) + ((0.0034494400000000002 * fTemp155) + ((0.0014532900000000001 * fTemp156) + ((0.0011545000000000001 * fTemp157) + ((0.00061171300000000006 * fTemp158) + ((0.0013395900000000001 * fTemp159) + ((0.00020694100000000001 * fTemp160) + ((0.00127111 * fTemp161) + ((0.00064708400000000003 * fTemp162) + ((0.0010632999999999999 * fTemp163) + ((0.00058977500000000006 * fTemp164) + ((0.000609699 * fTemp165) + ((0.00150508 * fTemp166) + ((0.0014054500000000001 * fTemp167) + ((0.0060181399999999999 * fTemp168) + ((0.0038672100000000003 * fTemp169) + ((0.0049045500000000006 * fTemp170) + ((0.0091428700000000009 * fTemp171) + ((4.1971900000000004e-05 * fTemp172) + ((0.0099856200000000006 * fTemp173) + ((0.010922899999999999 * fTemp174) + ((0.0016006100000000001 * fTemp175) + ((0.0013328999999999999 * fTemp176) + ((0.013603000000000001 * fTemp177) + ((0.0099153599999999998 * fTemp178) + ((0.016723499999999999 * fTemp179) + ((0.045127499999999994 * fTemp180) + ((0.0027371000000000001 * fTemp181) + ((0.0059659600000000002 * fTemp182) + ((4.6184399999999998e-05 * fTemp183) + ((0.0025209700000000004 * fTemp184) + ((0.00036855699999999999 * fTemp185) + ((0.00070276500000000005 * fTemp186) + ((0.00090358699999999997 * fTemp187) + ((0.000231406 * fTemp188) + ((4.4178200000000003e-05 * fTemp189) + ((1.57811e-05 * fTemp80) + ((0.0047945900000000005 * fTemp190) + ((8.2140799999999984e-06 * fTemp192) + ((0.0029068700000000002 * fTemp193) + ((3.8470500000000004e-05 * fTemp194) + ((9.096330000000001e-05 * fTemp195) + ((0.000227166 * fTemp196) + ((6.1820100000000001e-06 * fTemp197) + ((8.7317000000000004e-05 * fTemp198) + ((8.2866000000000002e-05 * fTemp199) + ((0.00014514899999999999 * fTemp200) + ((8.3273200000000014e-05 * fTemp201) + ((5.2586900000000003e-05 * fTemp202) + ((0.00036701000000000003 * fTemp203) + ((0.00010655300000000001 * fTemp204) + ((0.00050770799999999994 * fTemp205) + ((2.5137800000000002e-07 * fTemp206) + ((3.82797e-06 * fTemp207) + ((0.00025426099999999998 * fTemp208) + ((0.00018379600000000001 * fTemp209) + ((0.00035138200000000002 * fTemp210) + ((0.00063264199999999999 * fTemp211) + ((7.6149300000000013e-05 * fTemp212) + ((0.00086251900000000007 * fTemp213) + ((0.0019024199999999999 * fTemp214) + ((0.0013118799999999999 * fTemp215) + ((0.00129144 * fTemp216) + ((0.0015575400000000001 * fTemp217) + ((0.00020428199999999999 * fTemp218) + ((0.0011614100000000001 * fTemp219) + ((0.0020513699999999998 * fTemp220) + ((0.0016051100000000001 * fTemp221) + ((0.00196889 * fTemp222) + ((0.0079808199999999996 * fTemp223) + ((0.0044432300000000003 * fTemp224) + ((0.0013145500000000001 * fTemp225) + ((0.000208879 * fTemp226) + ((9.226730000000001e-05 * fTemp227) + ((0.0053043999999999999 * fTemp228) + ((0.0053560100000000005 * fTemp229) + ((0.00291413 * fTemp230) + ((0.010230900000000001 * fTemp231) + ((0.0073240200000000005 * fTemp232) + ((0.0073238899999999996 * fTemp233) + ((0.0105234 * fTemp234) + ((4.8748200000000002e-05 * fTemp235) + ((0.018119700000000002 * fTemp236) + ((0.0016041499999999999 * fTemp237) + ((2.7286800000000004e-05 * fTemp238) + ((0.00127749 * fTemp239) + ((0.00070097300000000003 * fTemp240) + ((0.0014662800000000001 * fTemp241) + ((0.0025716599999999999 * fTemp242) + ((0.0027539400000000003 * fTemp243) + ((0.000547636 * fTemp244) + ((0.00020467599999999999 * fTemp245) + ((0.0028814800000000001 * fTemp246) + ((0.0034324300000000002 * fTemp247) + ((0.0030387000000000001 * fTemp248) + ((0.002183 * fTemp249) + ((0.021556799999999997 * fTemp250) + ((0.0018844000000000001 * fTemp251) + ((0.013752100000000001 * fTemp252) + ((0.0448946 * fTemp253) + ((0.00033159299999999998 * fTemp254) + ((0.00255288 * fTemp255) + ((0.00094661300000000001 * fTemp256) + ((0.0010133200000000001 * fTemp257) + ((0.00025746299999999998 * fTemp258) + ((2.1185900000000001e-06 * fTemp259) + ((0.00014839199999999998 * fTemp260) + ((0.00047882000000000003 * fTemp261) + ((0.00147938 * fTemp262) + ((0.00013115699999999999 * fTemp263) + ((0.0011073599999999999 * fTemp264) + ((1.70273e-06 * fTemp266) + ((0.00050519600000000007 * fTemp267) + ((0.0016339300000000002 * fTemp268) + ((8.0341699999999997e-06 * fTemp269) + ((4.3469500000000001e-05 * fTemp270) + ((0.0022532999999999997 * fTemp271) + ((0.0024114399999999999 * fTemp272) + ((0.0048108300000000003 * fTemp273) + ((0.036438700000000004 * fTemp274) + ((0.027385400000000001 * fTemp275) + ((0.040978300000000002 * fTemp276) + ((0.038770699999999998 * fTemp277) + ((0.10758200000000001 * fTemp278) + ((0.049985600000000005 * fTemp279) + (((0.00010309 * fTemp280) + ((0.00018337600000000001 * fTemp281) + ((0.00108661 * fTemp282) + ((0.00040080500000000001 * fTemp283) + ((0.0025276200000000004 * fTemp284) + ((0.0005591420000000001 * fTemp285) + ((0.00045614100000000008 * fTemp287) + ((0.00160801 * fTemp288) + ((0.00084856599999999993 * fTemp289) + ((0.00020222799999999999 * fTemp290) + ((2.0016700000000001e-05 * fTemp291) + ((1.4622100000000002e-05 * fTemp191) + ((0.031673199999999999 * fTemp292) + ((0.0274497 * fTemp293) + ((0.0030908899999999998 * fTemp294) + ((0.0103427 * fTemp295) + ((0.0120915 * fTemp296) + ((0.0038904299999999998 * fTemp297) + ((0.0024724199999999999 * fTemp298) + ((0.0012886100000000001 * fTemp299) + ((0.000101833 * fTemp300) + ((2.4533100000000001e-06 * fTemp301) + ((0.0034125200000000005 * fTemp302) + ((0.0069197900000000003 * fTemp303) + ((0.000105639 * fTemp304) + ((3.9296600000000006e-05 * fTemp305) + ((3.85491e-05 * fTemp306) + ((0.00018456300000000003 * fTemp307) + ((0.0031713800000000001 * fTemp308) + ((0.0019589099999999999 * fTemp309) + ((0.00068253400000000005 * fTemp310) + ((0.00056285600000000003 * fTemp311) + ((0.0014036900000000002 * fTemp312) + ((0.0015441700000000001 * fTemp313) + ((0.00036768800000000002 * fTemp314) + ((0.00337025 * fTemp315) + ((0.00069545300000000007 * fTemp316) + ((0.00113331 * fTemp317) + ((0.00070883900000000006 * fTemp318) + ((0.0020663499999999998 * fTemp319) + ((0.00288332 * fTemp320) + ((0.0014698599999999999 * fTemp321) + ((0.0010311999999999999 * fTemp322) + ((0.0012247 * fTemp323) + ((0.00117636 * fTemp324) + ((0.0016634199999999999 * fTemp325) + ((0.0008817050000000001 * fTemp326) + ((0.0035919900000000002 * fTemp327) + ((0.0033690899999999999 * fTemp328) + ((0.0053477200000000003 * fTemp329) + ((0.00571347 * fTemp330) + ((0.0081425600000000001 * fTemp331) + ((0.0048383499999999999 * fTemp332) + ((0.0011593500000000002 * fTemp333) + ((0.00341282 * fTemp334) + ((0.0060068300000000003 * fTemp335) + ((0.0053672899999999994 * fTemp336) + ((1.73416e-05 * fTemp337) + ((0.0096774400000000007 * fTemp338) + ((0.03066 * fTemp339) + ((0.0019489700000000002 * fTemp340) + ((9.4065400000000002e-05 * fTemp341) + ((0.0068109299999999998 * fTemp342) + ((0.00503769 * fTemp343) + ((2.4375700000000001e-05 * fTemp345) + ((8.0066800000000004e-05 * fTemp346) + ((9.0816700000000014e-05 * fTemp347) + ((9.9426100000000014e-05 * fTemp348) + ((5.80835e-05 * fTemp349) + ((9.5024799999999997e-07 * fTemp350) + ((7.7452699999999998e-05 * fTemp351) + ((8.464169999999999e-06 * fTemp352) + ((5.1149400000000002e-05 * fTemp353) + ((0.00032476299999999999 * fTemp354) + ((0.0012872900000000002 * fTemp355) + ((0.0022912900000000001 * fTemp356) + ((0.0019518999999999999 * fTemp357) + ((0.00119739 * fTemp358) + ((0.0043689900000000005 * fTemp359) + ((0.00375801 * fTemp360) + ((0.0016994499999999999 * fTemp361) + ((0.0032713800000000004 * fTemp362) + ((0.0036832600000000003 * fTemp363) + ((0.00028545500000000005 * fTemp364) + ((0.0020680200000000003 * fTemp365) + ((0.00025900400000000002 * fTemp366) + ((0.00089328000000000009 * fTemp367) + ((0.0016691100000000001 * fTemp368) + ((3.3718100000000005e-05 * fTemp369) + ((0.00099548200000000001 * fTemp370) + ((0.00089519800000000015 * fTemp371) + ((0.00079125400000000002 * fTemp372) + ((0.00072158200000000002 * fTemp373) + ((0.0024358800000000001 * fTemp374) + ((0.0027909200000000001 * fTemp375) + ((0.0042173100000000002 * fTemp376) + ((0.0091871399999999999 * fTemp377) + ((0.0056765999999999995 * fTemp378) + ((0.0016214599999999999 * fTemp379) + ((0.00077908600000000012 * fTemp380) + ((0.0021427500000000001 * fTemp381) + ((0.0028589500000000003 * fTemp382) + ((0.0013077200000000001 * fTemp383) + ((0.0022329500000000005 * fTemp384) + ((0.0053105900000000004 * fTemp385) + ((0.0030150300000000001 * fTemp386) + ((0.0070971000000000003 * fTemp387) + ((0.0037660800000000002 * fTemp388) + ((0.00053429399999999994 * fTemp389) + ((0.0164669 * fTemp390) + ((0.0086483100000000011 * fTemp391) + ((0.021576300000000003 * fTemp392) + ((0.021685099999999999 * fTemp393) + ((0.0057622799999999998 * fTemp394) + ((0.0057969700000000002 * fTemp395) + ((0.0197594 * fTemp396) + ((0.0121098 * fTemp397) + ((0.032535399999999999 * fTemp398) + ((0.00050339700000000005 * fTemp399) + ((0.00086802899999999998 * fTemp400) + ((0.0016583500000000001 * fTemp401) + ((0.00031507000000000003 * fTemp402) + ((0.00049461800000000005 * fTemp403) + ((0.00015840400000000001 * fTemp404) + ((2.8519800000000004e-05 * fTemp405) + ((4.4578800000000004e-06 * fTemp344) + ((1.1796500000000001e-05 * fTemp406) + ((5.3605900000000008e-05 * fTemp407) + ((0.00047704000000000008 * fTemp408) + ((0.00043236800000000009 * fTemp409) + ((0.00046969700000000004 * fTemp410) + ((0.00092227100000000001 * fTemp411) + ((0.0013659500000000001 * fTemp412) + ((0.00108099 * fTemp413) + ((6.7984800000000007e-05 * fTemp414) + ((0.00070973900000000003 * fTemp415) + ((0.000705385 * fTemp416) + ((0.00045975200000000007 * fTemp417) + ((0.0013345100000000001 * fTemp418) + ((0.0013671599999999999 * fTemp419) + ((0.00088433700000000001 * fTemp420) + ((0.00081300600000000002 * fTemp421) + ((0.0010498400000000002 * fTemp422) + ((0.00300031 * fTemp423) + ((0.00214314 * fTemp424) + ((2.3648600000000003e-05 * fTemp425) + ((0.00058793100000000009 * fTemp426) + ((0.0017765299999999999 * fTemp427) + ((0.000264467 * fTemp428) + ((0.000623519 * fTemp429) + ((0.00033132700000000002 * fTemp430) + ((0.0016777999999999999 * fTemp431) + ((5.0813899999999995e-06 * fTemp433) + ((4.2541300000000006e-05 * fTemp434) + ((5.7126099999999992e-06 * fTemp435) + ((0.00016404399999999999 * fTemp436) + ((0.00010654600000000001 * fTemp437) + ((1.8717900000000003e-05 * fTemp438) + ((0.00016872499999999999 * fTemp439) + ((0.00067625599999999997 * fTemp440) + ((0.00053358400000000003 * fTemp441) + ((0.00083451500000000008 * fTemp442) + ((0.00098166300000000002 * fTemp443) + ((0.00050102000000000002 * fTemp444) + ((0.00054736599999999998 * fTemp445) + ((0.0009436510000000001 * fTemp446) + ((0.0014059200000000002 * fTemp447) + ((0.0012065399999999999 * fTemp448) + ((0.00154024 * fTemp449) + ((0.00185348 * fTemp450) + ((0.00058470199999999999 * fTemp451) + ((0.00050683400000000006 * fTemp452) + ((0.00087727600000000008 * fTemp453) + ((0.00054874000000000004 * fTemp454) + ((0.00272945 * fTemp455) + ((2.3955000000000004e-05 * fTemp456) + ((0.0021461499999999999 * fTemp457) + ((0.00061282500000000011 * fTemp458) + ((0.0037083100000000003 * fTemp459) + ((0.0016799500000000001 * fTemp460) + ((0.0051757600000000006 * fTemp461) + ((0.00041329900000000006 * fTemp462) + ((0.0041945800000000007 * fTemp463) + ((0.00056369199999999997 * fTemp464) + ((0.0015701700000000001 * fTemp465) + ((0.0063743100000000002 * fTemp466) + ((0.0052356299999999998 * fTemp467) + ((0.0029129100000000003 * fTemp468) + ((0.0032107500000000001 * fTemp469) + ((0.0030868900000000001 * fTemp470) + ((0.00185049 * fTemp471) + ((0.0014647 * fTemp472) + ((0.0022559300000000002 * fTemp473) + ((0.0034284599999999999 * fTemp474) + ((0.0023089200000000003 * fTemp475) + ((0.0027403499999999999 * fTemp476) + ((0.0042152999999999999 * fTemp477) + ((0.0068040700000000006 * fTemp478) + ((0.0062385100000000001 * fTemp479) + ((0.0035775299999999998 * fTemp480) + ((0.0042250600000000001 * fTemp481) + ((0.0031211899999999998 * fTemp482) + ((0.0012461600000000001 * fTemp483) + ((0.0012118500000000002 * fTemp484) + ((0.00305273 * fTemp485) + ((0.00325793 * fTemp486) + ((0.0028833999999999999 * fTemp487) + ((0.0079248200000000008 * fTemp488) + ((0.015005800000000001 * fTemp489) + ((0.0028857800000000001 * fTemp490) + ((0.0074329900000000004 * fTemp491) + ((0.0056945699999999995 * fTemp492) + ((0.0064973000000000001 * fTemp493) + ((0.011589100000000001 * fTemp494) + ((0.0069301900000000001 * fTemp495) + ((0.0087064099999999995 * fTemp496) + ((0.0266261 * fTemp497) + ((0.031746299999999998 * fTemp498) + ((0.030624600000000002 * fTemp499) + ((0.0528391 * fTemp500) + ((0.021260899999999999 * fTemp501) + ((0.096895100000000012 * fTemp502) + ((0.057809200000000005 * fTemp503) + ((0.06596840000000001 * fTemp504) + ((0.055772500000000003 * fTemp505) + ((0.034514800000000005 * fTemp506) + ((0.059606500000000007 * fTemp507) + ((0.073495100000000008 * fTemp508) + ((0.064749100000000004 * fTemp509) + ((0.0449074 * fTemp510) + ((0.0025590399999999998 * fTemp511) + ((0.00046662400000000006 * fTemp512) + ((0.00069381500000000008 * fTemp513) + ((7.7608600000000008e-05 * fTemp514) + ((0.00215198 * fTemp515) + ((0.000292148 * fTemp516) + ((0.00055458199999999997 * fTemp517) + ((0.00356967 * fTemp518) + ((0.0018462400000000001 * fTemp519) + ((0.0017431900000000001 * fTemp520) + ((0.000219724 * fTemp521) + ((0.0038023500000000003 * fTemp522) + ((3.8722400000000007e-05 * fTemp523) + ((2.7248300000000001e-05 * fTemp524) + ((0.0026238199999999998 * fTemp525) + ((8.7116700000000006e-05 * fTemp526) + ((0.0014712700000000002 * fTemp527) + ((0.000702438 * fTemp528) + ((0.0174746 * fTemp529) + ((0.021190899999999999 * fTemp530) + ((0.0025345699999999999 * fTemp531) + ((0.0025992000000000003 * fTemp532) + ((0.0061892400000000004 * fTemp533) + ((0.0020100199999999999 * fTemp534) + ((0.00148817 * fTemp535) + ((0.00053443399999999997 * fTemp536) + ((0.00069470999999999997 * fTemp537) + ((0.00013874000000000002 * fTemp538) + ((2.49699e-05 * fTemp539) + ((9.9362800000000004e-06 * fTemp265) + ((0.0027816299999999998 * fTemp540) + ((0.00038337700000000001 * fTemp541) + ((0.0016079900000000001 * fTemp542) + ((0.00046795000000000002 * fTemp543) + ((0.0016152100000000002 * fTemp544) + ((0.00010376700000000001 * fTemp545) + ((0.0024365300000000001 * fTemp546) + ((2.0625300000000001e-06 * fTemp547) + ((2.09787e-05 * fTemp548) + ((0.0041905200000000005 * fTemp549) + ((0.0024405 * fTemp550) + ((0.00082847699999999999 * fTemp551) + ((0.00306785 * fTemp552) + ((0.00175234 * fTemp553) + ((0.00250454 * fTemp554) + ((0.049342300000000006 * fTemp555) + ((0.056510400000000002 * fTemp556) + ((0.0036302599999999997 * fTemp557) + ((0.00050879500000000004 * fTemp558) + ((0.0011589199999999999 * fTemp559) + ((0.00024957099999999997 * fTemp560) + ((0.0018634200000000002 * fTemp561) + ((0.00014117900000000001 * fTemp562) + ((0.00076772799999999997 * fTemp563) + ((0.00059949000000000005 * fTemp564) + ((0.00096838400000000008 * fTemp565) + ((0.00012370799999999999 * fTemp566) + ((2.5118200000000003e-05 * fTemp432) + ((3.8827600000000008e-05 * fTemp567) + ((0.036282299999999996 * fTemp568) + ((0.000184012 * fTemp569) + ((0.0052786200000000004 * fTemp570) + ((8.8836499999999992e-06 * fTemp571) + ((8.7967399999999994e-06 * fTemp572) + ((2.4082099999999999e-06 * fTemp573) + ((0.00041981599999999999 * fTemp574) + ((0.00025855300000000002 * fTemp575) + ((0.00034160000000000001 * fTemp576) + ((0.00086377000000000014 * fTemp577) + ((0.0036652500000000001 * fTemp578) + ((0.058508199999999996 * fTemp579) + ((0.0214629 * fTemp580) + ((0.041341200000000002 * fTemp581) + ((0.050635700000000006 * fTemp582) + ((0.088954799999999987 * fTemp583) + ((0.029345699999999999 * fTemp584) + ((0.063658400000000004 * fTemp585) + ((0.078143400000000002 * fTemp586) + ((0.077131900000000003 * fTemp587) + ((0.060886200000000001 * fTemp588) + ((0.0054581200000000003 * fTemp589) + ((0.090551499999999993 * fTemp590) + ((0.0018110400000000001 * fTemp591) + ((0.00194177 * fTemp592) + ((0.00114689 * fTemp593) + ((0.00058690300000000006 * fTemp594) + ((0.0012299100000000001 * fTemp595) + ((0.00015469 * fTemp596) + ((3.3955100000000003e-05 * fTemp286) + ((0.0012923000000000001 * fTemp597) + ((0.00136699 * fTemp598) + ((0.00045382400000000002 * fTemp599) + ((0.0036129299999999999 * fTemp600) + ((0.0050028199999999998 * fTemp601) + ((0.0079812000000000008 * fTemp602) + ((0.0044680700000000002 * fTemp603) + ((0.0141227 * fTemp604) + ((0.020023599999999999 * fTemp605) + ((0.023001999999999998 * fTemp606) + ((0.0057082499999999998 * fTemp607) + ((0.011366300000000001 * fTemp608) + (((0.0070393900000000004 * fTemp609) + ((0.0048936500000000003 * fTemp610) + ((0.0095701499999999995 * fTemp611) + ((0.011886499999999999 * fTemp612) + ((0.015800600000000001 * fTemp613) + ((0.024750999999999999 * fTemp614) + ((0.0058739299999999994 * fTemp615) + ((8.8419900000000004e-05 * fTemp616) + (0.0086730599999999998 * fTemp617))))))))) + (5.2591200000000006e-05 * fTemp618))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) + (0.021392099999999997 * fTemp619)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) - ((2.1103299999999999e-06 * fTemp620) + ((1.9649999999999998e-06 * fTemp621) + ((5.5217400000000003e-06 * fTemp622) + ((3.11789e-06 * fTemp623) + ((0.00016924900000000001 * fTemp624) + ((0.00014172800000000001 * (fTemp625 - fTemp626)) + ((0.00016311100000000002 * fTemp627) + ((0.000138245 * fTemp628) + ((0.000478309 * fTemp629) + ((0.00021442500000000002 * fTemp630) + ((0.00030905200000000004 * fTemp631) + ((0.00091847499999999998 * fTemp632) + ((0.0013005599999999999 * fTemp633) + ((0.0010940099999999999 * fTemp634) + ((0.00026133000000000005 * fTemp635) + ((0.0020077300000000001 * fTemp636) + ((0.0032089100000000001 * fTemp637) + ((0.0044288800000000005 * fTemp638) + ((0.0010079800000000001 * fTemp639) + ((0.00761051 * fTemp640) + ((0.0075612199999999996 * fTemp641) + ((0.00277838 * fTemp642) + ((0.0047678900000000003 * fTemp643) + ((0.0024688500000000003 * fTemp644) + ((0.00063087099999999997 * fTemp645) + ((0.0018776900000000002 * fTemp646) + ((0.0082384199999999998 * fTemp647) + ((0.0050166199999999994 * fTemp648) + ((0.0028042699999999998 * fTemp649) + ((0.0249734 * fTemp650) + ((0.0102805 * fTemp651) + ((0.0112762 * fTemp652) + ((0.0070063399999999998 * fTemp653) + ((9.6015999999999997e-06 * fTemp654) + ((0.00016569499999999999 * fTemp655) + ((0.000131304 * fTemp656) + ((7.4421400000000012e-05 * fTemp657) + ((0.0015964400000000002 * fTemp658) + ((9.0750500000000005e-05 * fTemp659) + ((0.0023870700000000003 * fTemp660) + ((0.0018691199999999999 * fTemp661) + ((0.0014690600000000001 * fTemp662) + ((0.00020293100000000003 * fTemp663) + ((0.00042256999999999999 * fTemp664) + ((4.8196899999999999e-05 * fTemp665) + ((0.00081876599999999996 * fTemp666) + ((0.0012919000000000001 * fTemp667) + ((0.00164123 * fTemp668) + ((0.0035543000000000003 * fTemp669) + ((0.0019447000000000002 * fTemp670) + ((0.0045421799999999998 * fTemp671) + ((0.0032057800000000001 * fTemp672) + ((0.0098560599999999998 * fTemp673) + ((0.0039102499999999997 * fTemp674) + ((0.0055212400000000002 * fTemp675) + ((0.0014029000000000001 * fTemp676) + ((0.015326500000000002 * fTemp677) + ((0.0039000900000000002 * fTemp678) + ((0.0107983 * fTemp679) + ((0.0115642 * fTemp680) + ((0.0016720200000000002 * fTemp681) + ((9.0368300000000007e-05 * fTemp682) + ((0.00113754 * fTemp683) + ((0.0026714199999999999 * fTemp684) + ((0.00024966300000000001 * fTemp685) + ((0.0021342599999999998 * fTemp686) + ((0.00060378299999999999 * fTemp687) + ((0.00040582000000000004 * fTemp688) + ((0.00225807 * fTemp689) + ((0.0023507699999999999 * fTemp690) + ((0.0046170200000000003 * fTemp691) + ((0.015667299999999999 * fTemp692) + ((0.0121152 * fTemp693) + ((0.0031788599999999999 * fTemp694) + ((0.014456700000000001 * fTemp695) + ((0.043969699999999994 * fTemp696) + ((0.010103500000000001 * fTemp697) + ((0.026084999999999997 * fTemp698) + ((0.040577500000000002 * fTemp699) + ((0.0703236 * fTemp700) + ((0.086424600000000004 * fTemp701) + ((0.00526349 * fTemp702) + ((0.0021171000000000002 * fTemp703) + ((0.00027211800000000001 * fTemp704) + ((0.00074305500000000006 * fTemp705) + ((0.00070165199999999998 * fTemp706) + ((7.2115300000000004e-05 * fTemp707) + ((1.68655e-05 * fTemp0) + ((9.8288800000000007e-05 * fTemp708) + ((0.00119961 * fTemp709) + ((0.0011488700000000002 * fTemp710) + ((0.0028610799999999998 * fTemp711) + ((0.0011123800000000001 * fTemp712) + ((0.00013187999999999999 * fTemp713) + ((9.0214000000000001e-05 * fTemp714) + ((0.00048376700000000002 * fTemp715) + ((0.0128167 * fTemp716) + ((0.0026388100000000001 * fTemp717) + ((8.3314900000000018e-05 * fTemp718) + ((1.03751e-05 * fTemp719) + ((0.00040982500000000005 * fTemp720) + ((0.0083424600000000012 * fTemp721) + ((0.0016058699999999999 * fTemp722) + ((0.00574575 * fTemp723) + ((0.0048227900000000004 * fTemp724) + ((0.0053663600000000006 * fTemp725) + ((0.0110708 * fTemp726) + ((0.0074311999999999998 * fTemp727) + ((0.00037238400000000002 * fTemp728) + ((0.00252045 * fTemp729) + ((0.00052853700000000004 * fTemp730) + ((2.8269799999999997e-06 * fTemp731) + ((0.00017706000000000002 * fTemp732) + ((0.00017067000000000002 * fTemp733) + ((4.1155299999999994e-06 * fTemp734) + ((2.8368800000000002e-05 * fTemp735) + ((4.5063100000000006e-05 * fTemp736) + ((8.4151000000000001e-06 * fTemp737) + ((0.00012787900000000001 * fTemp738) + ((0.00010922300000000001 * fTemp739) + ((0.00014129499999999999 * fTemp740) + ((8.4602100000000008e-05 * fTemp741) + ((0.00010810400000000001 * fTemp742) + ((6.1171800000000003e-05 * fTemp743) + ((0.00102436 * fTemp744) + ((0.00100778 * fTemp745) + ((0.00108387 * fTemp746) + ((0.00035581800000000004 * fTemp747) + ((0.00192194 * fTemp748) + ((0.00355741 * fTemp749) + ((0.00203951 * fTemp750) + ((0.0044921400000000004 * fTemp751) + ((0.00099792900000000005 * fTemp752) + ((0.0026953699999999999 * fTemp753) + ((0.00069267700000000005 * fTemp754) + ((0.0014122200000000001 * fTemp755) + ((0.00053443700000000002 * fTemp756) + ((0.00059297400000000002 * fTemp757) + ((0.0013940900000000002 * fTemp758) + ((0.00086068200000000009 * fTemp759) + ((0.0012833600000000001 * fTemp760) + ((0.0050443700000000003 * fTemp761) + ((0.0050713199999999998 * fTemp762) + ((0.0031110899999999999 * fTemp763) + ((0.0032877399999999999 * fTemp764) + ((0.00909628 * fTemp765) + ((0.00419002 * fTemp766) + ((0.0073180300000000005 * fTemp767) + ((0.0036924600000000003 * fTemp768) + ((0.0046017100000000002 * fTemp769) + ((0.0070144200000000004 * fTemp770) + ((0.0020962000000000003 * fTemp771) + ((0.0052343400000000005 * fTemp772) + ((0.0071723899999999998 * fTemp773) + ((0.0036162899999999999 * fTemp774) + ((1.70851e-05 * fTemp775) + ((3.0657300000000002e-05 * fTemp776) + ((9.1851000000000012e-05 * fTemp777) + ((2.7205700000000003e-05 * fTemp778) + ((5.3340699999999993e-07 * fTemp779) + ((0.00062551900000000005 * fTemp780) + ((0.0032921200000000004 * fTemp781) + ((0.00152143 * fTemp782) + ((0.00193753 * fTemp783) + ((0.00096127300000000012 * fTemp784) + ((0.0042903999999999998 * fTemp785) + ((0.00047510500000000003 * fTemp786) + ((0.0013881099999999999 * fTemp787) + ((0.0037726500000000002 * fTemp788) + ((0.0017495700000000002 * fTemp789) + ((0.0095376600000000016 * fTemp790) + ((0.0099954400000000013 * fTemp791) + ((0.00354222 * fTemp792) + ((0.00581921 * fTemp793) + ((0.00064315100000000001 * fTemp794) + ((0.016502200000000002 * fTemp795) + ((0.026153100000000002 * fTemp796) + ((0.015518799999999999 * fTemp797) + ((0.011748099999999999 * fTemp798) + ((0.024044800000000002 * fTemp799) + ((0.011073200000000002 * fTemp800) + ((0.00016208700000000002 * fTemp801) + ((0.00023927800000000002 * fTemp802) + ((0.00099840900000000002 * fTemp803) + ((0.00026075800000000004 * fTemp804) + ((0.0017748 * fTemp805) + ((1.9893500000000002e-05 * fTemp806) + ((3.7877500000000005e-05 * fTemp807) + ((0.00031515600000000003 * fTemp808) + ((0.00068420899999999997 * fTemp809) + ((0.00044415800000000005 * fTemp810) + ((0.00142376 * fTemp811) + ((0.00049268100000000002 * fTemp812) + ((0.00166752 * fTemp813) + ((0.00127253 * fTemp814) + ((0.00033490500000000003 * fTemp815) + ((0.0017284399999999999 * fTemp816) + ((0.0014551499999999999 * fTemp817) + ((0.0011273899999999998 * fTemp818) + ((6.2119200000000007e-05 * fTemp819) + ((0.00027007400000000003 * fTemp820) + ((0.0019003900000000001 * fTemp821) + ((0.00078290800000000002 * fTemp822) + ((5.8659000000000002e-05 * fTemp823) + ((0.00034302399999999999 * fTemp824) + ((0.0030454200000000001 * fTemp825) + ((0.0010344 * fTemp826) + ((0.0021753000000000002 * fTemp827) + ((0.00241033 * fTemp828) + ((0.0054447699999999998 * fTemp829) + ((0.00192986 * fTemp830) + ((0.0027451799999999998 * fTemp831) + ((0.0050891900000000004 * fTemp832) + ((0.0039649200000000002 * fTemp833) + ((0.00053920599999999997 * fTemp834) + ((0.00034291200000000001 * fTemp835) + ((0.00023320700000000001 * fTemp836) + ((0.00041387999999999999 * fTemp837) + ((0.0013482500000000001 * fTemp838) + ((0.000120611 * fTemp839) + ((0.0040625900000000005 * fTemp840) + ((0.011856999999999999 * fTemp841) + ((0.0091953899999999995 * fTemp842) + ((0.0037140800000000002 * fTemp843) + ((0.0061361499999999999 * fTemp844) + ((0.0077227400000000005 * fTemp845) + ((2.6161300000000003e-05 * fTemp846) + ((0.00081881900000000014 * fTemp847) + ((0.00050415000000000008 * fTemp848) + ((0.00041316100000000005 * fTemp849) + ((0.00116132 * fTemp850) + ((0.0011271300000000001 * fTemp851) + ((0.0018522 * fTemp852) + ((0.00081971100000000009 * fTemp853) + ((0.0040265300000000004 * fTemp854) + ((0.011590800000000002 * fTemp855) + ((0.0038800599999999998 * fTemp856) + ((0.022263399999999999 * fTemp857) + ((0.020153300000000002 * fTemp858) + ((0.042437800000000005 * fTemp859) + ((0.047133599999999998 * fTemp860) + ((0.00567696 * fTemp861) + ((0.00143681 * fTemp862) + ((0.00086101400000000001 * fTemp863) + ((0.00058492199999999998 * fTemp864) + ((6.7829099999999997e-06 * fTemp865) + ((0.00025529600000000002 * fTemp866) + ((2.9285600000000003e-05 * fTemp867) + ((3.8661599999999993e-06 * fTemp71) + ((9.0296599999999999e-06 * fTemp868) + ((0.0015305800000000001 * fTemp869) + ((0.0017261400000000001 * fTemp870) + ((0.00048162000000000004 * fTemp871) + ((1.14088e-05 * fTemp872) + ((0.00071563500000000006 * fTemp873) + ((0.0038506399999999998 * fTemp874) + ((0.00191281 * fTemp875) + ((0.0015042599999999999 * fTemp876) + ((0.00011532 * fTemp877) + ((4.7581900000000006e-05 * fTemp878) + ((3.0118399999999995e-06 * fTemp879) + ((0.00026214100000000002 * fTemp880) + ((0.0017591500000000001 * fTemp881) + ((0.00237535 * fTemp882) + ((0.0055445599999999996 * fTemp883) + ((6.8184199999999995e-06 * fTemp884) + ((7.66841e-06 * fTemp885) + ((3.5872199999999999e-05 * fTemp886) + ((0.0020068299999999998 * fTemp887) + ((3.5305499999999996e-06 * fTemp888) + ((4.5684500000000006e-05 * fTemp889) + ((9.9956200000000017e-05 * fTemp890) + ((0.0011713800000000001 * fTemp891) + ((0.016606200000000002 * fTemp892) + ((0.016194200000000002 * fTemp893) + ((0.0116756 * fTemp894) + ((0.008875280000000001 * fTemp895) + ((0.022184499999999999 * fTemp896) + ((0.00091979399999999993 * fTemp897) + ((0.00050699800000000004 * fTemp898) + ((0.010792999999999999 * fTemp899) + ((0.014927200000000002 * fTemp900) + ((0.008510160000000001 * fTemp901) + ((0.0223719 * fTemp902) + ((0.0123166 * fTemp903) + ((0.017947399999999999 * fTemp904) + ((0.0073694100000000007 * fTemp905) + ((0.0142655 * fTemp906) + ((0.0067548000000000009 * fTemp907) + ((0.0025108100000000005 * fTemp908) + ((0.00235191 * fTemp909) + ((6.9716599999999994e-06 * fTemp910) + ((0.010143899999999999 * fTemp911) + ((9.8485200000000004e-06 * fTemp912) + ((0.00010357900000000001 * fTemp913) + ((0.00042897299999999997 * fTemp914) + ((9.8297200000000012e-05 * fTemp915) + ((0.00056389100000000007 * fTemp916) + ((0.00051816800000000006 * fTemp917) + ((0.00088825400000000011 * fTemp918) + ((0.000446193 * fTemp919) + ((0.00071199100000000003 * fTemp920) + ((0.0012435300000000001 * fTemp921) + ((0.0012468500000000001 * fTemp922) + ((0.00170268 * fTemp923) + ((0.0020163799999999999 * fTemp924) + ((0.0013548899999999999 * fTemp925) + ((0.0010388900000000002 * fTemp926) + ((0.00126004 * fTemp927) + ((0.00065456900000000003 * fTemp928) + ((0.00054109500000000001 * fTemp929) + ((0.00083962700000000002 * fTemp930) + ((0.00142103 * fTemp931) + ((0.0030592000000000002 * fTemp932) + ((0.00082976100000000015 * fTemp933) + ((0.0020040399999999999 * fTemp934) + ((0.0019008400000000002 * fTemp935) + ((0.00032308700000000001 * fTemp936) + ((0.00083032800000000008 * fTemp937) + ((0.00146794 * fTemp938) + ((0.0029912300000000001 * fTemp939) + ((4.5241600000000004e-05 * fTemp940) + ((0.00121188 * fTemp941) + ((0.00103439 * fTemp942) + ((0.00025587599999999999 * fTemp943) + ((0.00193403 * fTemp944) + ((0.0015286800000000001 * fTemp945) + ((0.000502175 * fTemp946) + ((0.00055048599999999999 * fTemp947) + ((0.0014905299999999999 * fTemp948) + ((0.0030320900000000003 * fTemp949) + ((0.0076587399999999998 * fTemp950) + ((0.0029968600000000001 * fTemp951) + ((0.010064699999999999 * fTemp952) + ((0.030136699999999999 * fTemp953) + ((0.039170099999999999 * fTemp954) + ((0.00031563800000000003 * fTemp955) + ((0.00069887100000000011 * fTemp956) + ((0.025458500000000002 * fTemp957) + ((0.00049892100000000004 * fTemp958) + ((0.0024887899999999998 * fTemp959) + ((6.574880000000001e-05 * fTemp960) + ((7.5893800000000015e-05 * fTemp961) + ((0.00055035400000000008 * fTemp962) + ((0.00038355300000000002 * fTemp963) + ((0.00064190699999999994 * fTemp964) + ((0.00096466700000000011 * fTemp965) + ((0.0012643700000000001 * fTemp966) + ((0.0014951399999999998 * fTemp967) + ((0.0013151 * fTemp968) + ((0.00147963 * fTemp969) + ((0.0012132899999999999 * fTemp970) + ((0.0010497099999999999 * fTemp971) + ((5.4447799999999994e-06 * fTemp972) + ((0.00062200899999999997 * fTemp973) + ((0.00026040700000000004 * fTemp974) + ((0.00038862600000000004 * fTemp975) + ((0.00155276 * fTemp976) + ((0.00031511100000000003 * fTemp977) + ((0.0011385200000000001 * fTemp978) + ((4.1098800000000007e-05 * fTemp979) + ((0.00191902 * fTemp980) + ((0.00073584900000000001 * fTemp981) + ((0.0033698500000000002 * fTemp982) + ((0.0023925600000000002 * fTemp983) + ((0.00177024 * fTemp984) + ((0.0015381499999999998 * fTemp985) + ((0.0027169400000000002 * fTemp986) + ((0.0011334100000000001 * fTemp987) + ((0.0046562499999999998 * fTemp988) + ((0.0026088700000000001 * fTemp989) + ((0.0012374899999999999 * fTemp990) + ((0.00029472900000000003 * fTemp991) + ((0.00350462 * fTemp992) + ((0.00161133 * fTemp993) + ((0.0043256300000000004 * fTemp994) + ((0.0039263700000000002 * fTemp995) + ((0.0193136 * fTemp996) + ((0.0137585 * fTemp997) + ((0.016165499999999999 * fTemp998) + ((0.0200181 * fTemp999) + ((0.00436148 * fTemp1000) + ((0.0115485 * fTemp1001) + ((0.0032127699999999998 * fTemp1002) + ((0.0057804000000000006 * fTemp1003) + ((0.037111700000000004 * fTemp1004) + ((0.014907300000000002 * fTemp1005) + ((0.0092683000000000001 * fTemp1006) + ((0.0158698 * fTemp1007) + ((0.00489909 * fTemp1008) + ((0.00036272099999999999 * fTemp1009) + ((0.00043871700000000003 * fTemp1010) + ((0.00018132700000000001 * fTemp1011) + ((0.00097058900000000011 * fTemp1012) + ((0.00016036100000000001 * fTemp1013) + ((0.00019245000000000002 * fTemp1014) + ((3.8801999999999999e-05 * fTemp1015) + ((6.5230500000000005e-05 * fTemp1016) + ((4.81323e-05 * fTemp1017) + ((3.1175000000000006e-05 * fTemp1018) + ((9.139900000000001e-05 * fTemp1019) + ((0.000137362 * fTemp1020) + ((0.000272806 * fTemp1021) + ((0.00025063300000000005 * fTemp1022) + ((0.000314307 * fTemp1023) + ((8.3185099999999991e-06 * fTemp1024) + ((1.1129000000000001e-05 * fTemp1025) + ((0.00012562199999999998 * fTemp1026) + ((0.00027303400000000002 * fTemp1027) + ((8.74662e-06 * fTemp1028) + ((0.00016385400000000001 * fTemp1029) + ((0.00026935700000000002 * fTemp1030) + ((0.00035653000000000003 * fTemp1031) + ((2.5639300000000001e-06 * fTemp1032) + ((0.0011749499999999999 * fTemp1033) + ((0.000138672 * fTemp1034) + ((0.00092974400000000003 * fTemp1035) + ((0.00189912 * fTemp1036) + ((0.00157916 * fTemp1037) + ((0.0017056100000000002 * fTemp1038) + ((0.00246014 * fTemp1039) + ((0.0047484400000000005 * fTemp1040) + ((0.010441600000000001 * fTemp1041) + ((0.0044470300000000003 * fTemp1042) + ((0.0100582 * fTemp1043) + ((0.0128559 * fTemp1044) + ((0.015119400000000002 * fTemp1045) + ((0.016079200000000002 * fTemp1046) + ((0.015913200000000002 * fTemp1047) + ((0.0076609499999999997 * fTemp1048) + ((0.0068764300000000002 * fTemp1049) + ((0.0095819500000000005 * fTemp1050) + ((0.0024969700000000003 * fTemp1051) + ((0.0014838199999999998 * fTemp1052) + ((0.000311911 * fTemp1053) + ((0.00054486499999999995 * fTemp1054) + ((0.0114164 * fTemp1055) + ((0.015415000000000002 * fTemp1056) + ((0.0023363800000000003 * fTemp1057) + ((0.0013234399999999999 * fTemp1058) + ((0.0047624399999999997 * fTemp1059) + ((0.0035146700000000001 * fTemp1060) + ((0.0014325799999999999 * fTemp1061) + ((0.00035963800000000001 * fTemp1062) + ((0.00013482199999999999 * fTemp1063) + ((0.0031359700000000001 * fTemp1064) + ((0.0033774600000000001 * fTemp1065) + ((0.00043131800000000003 * fTemp1066) + ((0.0013603899999999999 * fTemp1067) + ((0.00076972400000000006 * fTemp1068) + ((0.0011577500000000001 * fTemp1069) + ((0.0011045300000000001 * fTemp1070) + ((0.0027559100000000003 * fTemp1071) + ((0.0016313200000000001 * fTemp1072) + ((0.0019843199999999999 * fTemp1073) + ((0.0031219500000000001 * fTemp1074) + ((0.0017147500000000001 * fTemp1075) + ((0.0120184 * fTemp1076) + ((0.011691100000000001 * fTemp1077) + ((0.0053095800000000004 * fTemp1078) + ((0.0072171800000000001 * fTemp1079) + ((0.0030320200000000003 * fTemp1080) + ((0.00131485 * fTemp1081) + ((0.0011081700000000001 * fTemp1082) + ((0.00036208399999999998 * fTemp1083) + ((0.00017773600000000001 * fTemp1084) + ((0.00073753600000000001 * fTemp1085) + ((0.00060830200000000002 * fTemp1086) + ((0.00102903 * fTemp1087) + ((0.0030104600000000004 * fTemp1088) + ((0.0010694400000000001 * fTemp1089) + ((0.0029673899999999999 * fTemp1090) + ((0.0023879000000000001 * fTemp1091) + ((0.00017378000000000001 * fTemp1092) + ((0.00020804999999999999 * fTemp1093) + ((0.00029024100000000006 * fTemp1094) + ((0.0010302600000000001 * fTemp1095) + ((0.017341200000000001 * fTemp1096) + ((0.00280679 * fTemp1097) + ((0.00054230400000000007 * fTemp1098) + ((0.0010789999999999999 * fTemp1099) + ((0.00127535 * fTemp1100) + ((1.8656099999999998e-06 * fTemp1101) + ((0.0188738 * fTemp1102) + ((0.0027951100000000004 * fTemp1103) + ((0.0047547600000000002 * fTemp1104) + ((0.0037710400000000002 * fTemp1105) + ((0.0045419900000000001 * fTemp1106) + ((0.00141242 * fTemp1107) + ((0.00134669 * fTemp1108) + ((0.020373700000000002 * fTemp1109) + ((0.0086227300000000003 * fTemp1110) + ((0.015368999999999999 * fTemp1111) + ((0.026966700000000003 * fTemp1112) + ((0.023967600000000002 * fTemp1113) + ((0.025263300000000002 * fTemp1114) + ((0.019380700000000001 * fTemp1115) + ((0.0052166499999999998 * fTemp1116) + ((0.012525700000000001 * fTemp1117) + ((0.039007399999999998 * fTemp1118) + ((0.039406500000000004 * fTemp1119) + ((0.0160175 * fTemp1120) + ((0.035762599999999999 * fTemp1121) + ((0.026436999999999999 * fTemp1122) + ((0.0180915 * fTemp1123) + ((0.00060328500000000006 * fTemp1124) + ((0.042794800000000008 * fTemp1125) + ((0.034687500000000003 * fTemp1126) + ((0.0095142000000000004 * fTemp1127) + ((0.0011779100000000001 * fTemp1128) + ((0.000167329 * fTemp1129) + ((0.00041459900000000003 * fTemp1130) + ((0.00031506200000000001 * fTemp1131) + ((0.0021747000000000003 * fTemp1132) + ((0.0024374800000000001 * fTemp1133) + ((0.0018498099999999999 * fTemp1134) + ((0.010498400000000001 * fTemp1135) + ((0.0021260799999999998 * fTemp1136) + ((0.0010427100000000001 * fTemp1137) + ((0.0065524800000000003 * fTemp1138) + ((0.011496599999999999 * fTemp1139) + ((0.0092546800000000012 * fTemp1140) + ((0.00041918000000000001 * fTemp1141) + (0.0032369600000000001 * fTemp1142))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
			fRec0[0] = std::max<double>((fRec0[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp1143))))));
			fHbargraph0 = FAUSTFLOAT(fRec0[0]);
			output0[i] = FAUSTFLOAT(fTemp1143);
			double fTemp1144 = (fRec1[0] * (((1.07578e-06 * fTemp622) + ((1.0896600000000002e-05 * fTemp623) + ((5.7825300000000005e-06 * fTemp1) + ((3.3373e-05 * fTemp628) + ((0.0044664700000000002 * fTemp5) + ((0.0013042499999999999 * fTemp6) + ((0.0025056699999999998 * fTemp7) + ((0.0014331000000000001 * fTemp653) + ((3.3066799999999999e-06 * fTemp12) + ((9.6444300000000008e-05 * fTemp17) + ((0.00063805800000000005 * fTemp19) + ((0.0011957500000000002 * fTemp20) + ((0.00034096799999999998 * fTemp659) + ((0.0010047600000000002 * fTemp22) + ((0.00095472699999999994 * fTemp23) + ((0.0011230999999999999 * fTemp24) + ((0.000103257 * fTemp26) + ((0.00171093 * fTemp665) + ((0.0024843999999999999 * fTemp27) + ((0.0013637999999999999 * fTemp666) + ((0.0020383699999999999 * fTemp28) + ((0.00292619 * fTemp667) + ((0.00074355600000000003 * fTemp30) + ((0.0039499399999999999 * fTemp32) + ((0.0064549899999999999 * fTemp672) + ((0.0094244500000000009 * fTemp676) + ((0.019293299999999999 * fTemp35) + ((0.019155500000000002 * fTemp678) + ((0.0068834500000000002 * fTemp37) + ((0.036199000000000002 * fTemp38) + ((1.0336899999999999e-06 * fTemp40) + ((0.0020205099999999997 * fTemp42) + ((0.0022768000000000003 * fTemp43) + ((0.00027500499999999999 * fTemp45) + ((0.00010999399999999999 * fTemp46) + ((1.2511200000000001e-05 * fTemp47) + ((0.0018692700000000001 * fTemp48) + ((0.000129173 * fTemp49) + ((0.0010500000000000002 * fTemp50) + ((0.0037914799999999999 * fTemp51) + ((0.0063763400000000003 * fTemp52) + ((0.0065615300000000003 * fTemp53) + ((0.00222449 * fTemp54) + ((0.049093400000000002 * fTemp56) + ((0.009442299999999999 * fTemp694) + ((0.00046772600000000007 * fTemp695) + ((0.054047100000000001 * fTemp57) + ((0.0108471 * fTemp697) + ((0.0010471900000000001 * fTemp704) + ((0.00047446700000000006 * fTemp61) + ((0.00031437800000000003 * fTemp63) + ((1.5753899999999999e-07 * fTemp64) + ((0.00036215100000000001 * fTemp65) + ((0.00023396100000000001 * fTemp66) + ((0.00052374700000000008 * fTemp67) + ((0.00092775000000000008 * fTemp68) + ((0.00122649 * fTemp69) + ((0.00050830900000000008 * fTemp709) + ((0.00015973800000000001 * fTemp72) + ((0.00065440400000000003 * fTemp73) + ((0.00025875799999999999 * fTemp75) + ((0.00049362799999999997 * fTemp76) + ((0.00016541 * fTemp77) + ((0.0010192300000000001 * fTemp78) + ((0.00032049699999999999 * fTemp716) + ((0.034925999999999999 * fTemp79) + ((3.4760399999999997e-08 * fTemp81) + ((0.020153599999999997 * fTemp82) + ((0.015254500000000001 * fTemp717) + ((4.4900900000000009e-05 * fTemp718) + ((1.2673900000000001e-05 * fTemp719) + ((0.0099207400000000008 * fTemp89) + ((0.041638400000000006 * fTemp90) + ((0.028412400000000001 * fTemp722) + ((0.016930500000000001 * fTemp725) + ((0.0106017 * fTemp93) + ((0.000263627 * fTemp726) + ((0.00025129900000000002 * fTemp94) + ((0.00047378999999999998 * fTemp728) + ((0.000359551 * fTemp96) + ((0.00050858999999999995 * fTemp98) + ((0.00025865000000000003 * fTemp730) + ((5.1984399999999995e-06 * fTemp100) + ((2.0081900000000002e-05 * fTemp731) + ((5.35095e-06 * fTemp11) + ((0.00030013000000000001 * fTemp101) + ((0.00012962100000000001 * fTemp733) + ((0.00012994100000000001 * fTemp104) + ((0.000501459 * fTemp105) + ((0.00041380000000000003 * fTemp106) + ((4.2136500000000006e-05 * fTemp108) + ((1.1025e-06 * fTemp109) + ((0.000102185 * fTemp110) + ((0.0010346200000000002 * fTemp111) + ((0.0015065 * fTemp112) + ((2.1505000000000002e-05 * fTemp741) + ((0.00061807300000000002 * fTemp113) + ((6.5997900000000001e-08 * fTemp115) + ((7.1971000000000006e-05 * fTemp744) + ((0.00051516900000000011 * fTemp119) + ((0.00076406400000000006 * fTemp120) + ((9.4944000000000018e-05 * fTemp745) + ((0.00095418100000000002 * fTemp747) + ((0.00034814200000000002 * fTemp750) + ((8.0286900000000005e-05 * fTemp125) + ((0.00159566 * fTemp126) + ((0.0016114299999999999 * fTemp127) + ((1.1146700000000001e-05 * fTemp754) + ((0.0029336900000000001 * fTemp128) + ((0.0015251900000000001 * fTemp129) + ((0.00063413500000000008 * fTemp756) + ((0.00021672300000000002 * fTemp757) + ((0.0030693399999999998 * fTemp132) + ((7.2115600000000003e-07 * fTemp133) + ((2.0505700000000003e-05 * fTemp775) + ((7.2276399999999995e-06 * fTemp776) + ((4.1691399999999999e-05 * fTemp138) + ((0.00027668200000000005 * fTemp147) + ((1.3220600000000002e-05 * fTemp779) + ((0.0002275 * fTemp780) + ((0.0020786799999999999 * fTemp781) + ((0.00280241 * fTemp782) + ((0.00091981900000000011 * fTemp783) + ((7.5101900000000007e-05 * fTemp158) + ((0.0026788099999999998 * fTemp784) + ((0.0036546199999999999 * fTemp785) + ((0.00063752100000000005 * fTemp164) + ((0.00050336900000000004 * fTemp786) + ((0.0013227800000000002 * fTemp787) + ((7.3547000000000006e-05 * fTemp788) + ((0.00076943900000000002 * fTemp165) + ((0.00033734999999999999 * fTemp166) + ((0.0013802 * fTemp167) + ((0.0050838799999999998 * fTemp168) + ((0.013509400000000001 * fTemp171) + ((0.015784800000000002 * fTemp793) + ((0.00206245 * fTemp175) + ((0.0024348900000000003 * fTemp795) + ((0.0034120499999999998 * fTemp176) + ((0.036800600000000003 * fTemp796) + ((0.016894100000000002 * fTemp798) + ((0.015907999999999999 * fTemp799) + ((0.00139284 * fTemp181) + ((0.0111489 * fTemp800) + ((0.00048254200000000003 * fTemp183) + ((0.00084440200000000006 * fTemp803) + ((0.00014837300000000001 * fTemp804) + ((2.2315100000000003e-05 * fTemp806) + ((4.6366800000000005e-05 * fTemp625) + ((0.00015426800000000002 * fTemp204) + ((2.9996500000000003e-05 * fTemp205) + ((0.00011103000000000001 * fTemp808) + ((0.00013601100000000001 * fTemp208) + ((0.00055044600000000001 * fTemp209) + ((0.00044798200000000004 * fTemp210) + ((0.00057507100000000002 * fTemp811) + ((0.00032662200000000005 * fTemp812) + ((4.4411900000000001e-05 * fTemp814) + ((0.000103707 * fTemp815) + ((0.00100478 * fTemp816) + ((0.00012959000000000001 * fTemp818) + ((0.0012166300000000002 * fTemp819) + ((0.00022863600000000002 * fTemp820) + ((0.00033440100000000001 * fTemp219) + ((0.00098071900000000012 * fTemp821) + ((0.00040021800000000004 * fTemp823) + ((0.0016816299999999999 * fTemp222) + ((0.00099638500000000002 * fTemp825) + ((0.0034718700000000002 * fTemp827) + ((0.0068587300000000004 * fTemp225) + ((0.0058219500000000002 * fTemp829) + ((0.00087088200000000001 * fTemp234) + ((0.017078900000000001 * fTemp831) + ((0.027225600000000003 * fTemp832) + ((9.9177200000000003e-05 * fTemp838) + ((0.0006416280000000001 * fTemp839) + ((0.0071496000000000007 * fTemp236) + ((0.0043624700000000002 * fTemp237) + ((0.00055969600000000004 * fTemp849) + ((0.0028104300000000005 * fTemp238) + ((0.00089791299999999997 * fTemp850) + ((0.0015055300000000001 * fTemp241) + ((0.00129704 * fTemp242) + ((0.00042873700000000005 * fTemp244) + ((0.00077501600000000001 * fTemp247) + ((0.0177702 * fTemp248) + ((0.0011423000000000002 * fTemp249) + ((0.0057435900000000007 * fTemp854) + ((0.0096359600000000007 * fTemp250) + ((0.0037277 * fTemp251) + ((0.0069752599999999996 * fTemp252) + ((0.045202099999999995 * fTemp253) + ((0.00526948 * fTemp856) + ((0.000358363 * fTemp255) + ((0.00023124899999999999 * fTemp863) + ((0.00020135900000000002 * fTemp257) + ((0.00015575000000000002 * fTemp258) + ((6.1446399999999998e-06 * fTemp259) + ((8.1741199999999999e-06 * fTemp868) + ((0.0011816700000000001 * fTemp869) + ((0.00073013500000000003 * fTemp870) + ((0.00065044300000000001 * fTemp260) + ((0.0015448500000000002 * fTemp871) + ((0.00041635900000000004 * fTemp872) + ((0.00109789 * fTemp873) + ((0.0026210900000000004 * fTemp874) + ((0.0018869100000000001 * fTemp875) + ((3.3187600000000001e-06 * fTemp266) + ((0.0024495200000000002 * fTemp268) + ((0.0022621500000000001 * fTemp876) + ((8.7600299999999995e-06 * fTemp269) + ((2.9587800000000002e-05 * fTemp270) + ((0.0047280300000000003 * fTemp272) + ((0.0056186100000000004 * fTemp273) + ((0.041430000000000002 * fTemp274) + ((0.032932100000000006 * fTemp275) + ((0.028794800000000002 * fTemp883) + ((0.024621200000000003 * fTemp276) + ((0.06345640000000001 * fTemp277) + ((0.07541260000000001 * fTemp278) + (((1.0190999999999999e-05 * fTemp280) + ((0.00125953 * fTemp886) + ((0.00019295400000000001 * fTemp887) + (((0.0020002800000000001 * fTemp288) + ((0.0121522 * fTemp892) + ((0.011685700000000002 * fTemp893) + ((0.0122635 * fTemp894) + ((0.017027300000000002 * fTemp895) + ((0.017569399999999999 * fTemp896) + ((0.00062478100000000003 * fTemp897) + ((0.00028939400000000006 * fTemp898) + ((0.0047823300000000004 * fTemp899) + ((0.048044200000000002 * fTemp900) + ((0.031593799999999998 * fTemp902) + ((0.0092093100000000001 * fTemp903) + ((0.0182515 * fTemp904) + ((0.0125871 * fTemp294) + ((0.0010793700000000001 * fTemp905) + ((0.0061485900000000007 * fTemp906) + ((0.00108131 * fTemp907) + ((0.00149292 * fTemp909) + ((5.1482899999999996e-06 * fTemp910) + ((5.530960000000001e-05 * fTemp300) + ((1.4188099999999999e-06 * fTemp301) + ((0.00946403 * fTemp911) + ((0.0038781100000000002 * fTemp303) + ((0.00011918500000000002 * fTemp304) + ((9.8678100000000013e-05 * fTemp305) + ((4.2207000000000004e-05 * fTemp912) + ((3.2490500000000003e-05 * fTemp306) + ((0.00011697900000000002 * fTemp307) + ((0.000115798 * fTemp915) + ((0.0027517499999999999 * fTemp308) + ((0.0037339199999999999 * fTemp309) + ((0.00065997500000000004 * fTemp924) + ((0.0016790700000000002 * fTemp311) + ((0.0019817300000000001 * fTemp931) + ((0.0015044699999999999 * fTemp313) + ((0.0010922900000000001 * fTemp314) + ((0.0040626200000000003 * fTemp315) + ((0.0012104800000000001 * fTemp317) + ((0.00102589 * fTemp318) + ((0.0011136799999999999 * fTemp319) + ((0.0035448200000000002 * fTemp320) + ((0.0039074399999999999 * fTemp321) + ((0.00070128400000000004 * fTemp946) + ((0.00040873600000000004 * fTemp947) + ((0.00017855099999999999 * fTemp323) + ((0.0020778099999999998 * fTemp324) + ((0.0011952499999999999 * fTemp325) + ((0.0022150400000000002 * fTemp326) + ((3.8517100000000004e-05 * fTemp327) + ((0.011077500000000001 * fTemp328) + ((0.0020168600000000001 * fTemp329) + ((0.0022290399999999998 * fTemp331) + ((0.0084232300000000003 * fTemp332) + ((0.0011997199999999998 * fTemp951) + ((0.0125606 * fTemp336) + ((0.00127378 * fTemp337) + ((0.0537481 * fTemp339) + ((0.013059400000000001 * fTemp340) + ((0.021176 * fTemp341) + ((0.0048699899999999994 * fTemp958) + ((0.0061526599999999999 * fTemp959) + ((0.0028706900000000004 * fTemp343) + ((3.4954000000000004e-05 * fTemp345) + ((9.3229500000000019e-05 * fTemp346) + ((9.5701199999999998e-05 * fTemp347) + ((0.00012240800000000001 * fTemp348) + ((5.5900900000000005e-05 * fTemp960) + ((1.0045e-05 * fTemp352) + ((1.9102600000000002e-05 * fTemp961) + ((0.0020406000000000001 * fTemp357) + ((0.000876507 * fTemp972) + ((0.0013280499999999999 * fTemp359) + ((0.0017303100000000001 * fTemp360) + ((0.0029776099999999999 * fTemp361) + ((0.0025195700000000001 * fTemp362) + ((0.00102092 * fTemp977) + ((0.00082739399999999996 * fTemp365) + ((0.00174807 * fTemp366) + ((0.00013990400000000002 * fTemp979) + ((0.0021519 * fTemp368) + ((0.00235812 * fTemp369) + ((7.6654099999999985e-06 * fTemp987) + ((0.0022741699999999998 * fTemp991) + ((0.0011889400000000001 * fTemp373) + ((0.0042530099999999998 * fTemp374) + ((0.0079603199999999999 * fTemp376) + ((0.0030812000000000001 * fTemp377) + ((0.0075640600000000001 * fTemp993) + ((0.0032282000000000001 * fTemp378) + ((0.0041592900000000004 * fTemp379) + ((0.00033097600000000002 * fTemp994) + ((0.0066906600000000002 * fTemp382) + ((0.00018497500000000001 * fTemp383) + ((0.00108066 * fTemp385) + ((0.0081795500000000007 * fTemp387) + ((0.0040393699999999996 * fTemp388) + ((0.0088403900000000001 * fTemp389) + ((0.0054466000000000002 * fTemp390) + ((0.033863999999999998 * fTemp392) + ((0.0092350000000000002 * fTemp393) + ((0.0043864799999999999 * fTemp394) + ((0.00053732699999999999 * fTemp395) + ((0.0027836500000000004 * fTemp1002) + ((0.0092729000000000006 * fTemp1003) + ((0.022299000000000003 * fTemp396) + ((0.0012059800000000002 * fTemp397) + ((0.025815500000000002 * fTemp398) + ((7.0800700000000008e-05 * fTemp1011) + ((0.00076956600000000007 * fTemp401) + ((0.000231146 * fTemp402) + ((0.00014766099999999999 * fTemp403) + ((4.1470500000000002e-05 * fTemp1015) + ((7.1901800000000003e-05 * fTemp1016) + ((3.71676e-05 * fTemp1017) + ((0.00011888700000000002 * fTemp1019) + ((0.00015893600000000001 * fTemp1020) + ((1.5261800000000004e-05 * fTemp406) + ((0.00036830800000000003 * fTemp1021) + ((0.00030627700000000004 * fTemp1022) + ((0.000251322 * fTemp1023) + ((8.5058500000000009e-06 * fTemp1024) + ((3.3982700000000002e-05 * fTemp1025) + ((2.6751600000000003e-05 * fTemp1026) + ((0.000239084 * fTemp1027) + ((9.1238799999999995e-06 * fTemp1028) + ((0.00016550199999999999 * fTemp1029) + ((0.00022708 * fTemp1030) + ((3.6973199999999999e-06 * fTemp1032) + ((0.00015429699999999999 * fTemp414) + ((0.00065994200000000006 * fTemp427) + ((0.00021972700000000002 * fTemp428) + ((0.0011643300000000001 * fTemp430) + ((0.0017905 * fTemp1035) + ((0.00164631 * fTemp1037) + ((0.00227327 * fTemp1038) + ((0.0030369999999999998 * fTemp1039) + ((0.0020450900000000003 * fTemp1040) + ((0.010980500000000001 * fTemp1041) + ((0.0023284899999999999 * fTemp1042) + ((0.0074067100000000004 * fTemp1043) + ((0.016699200000000001 * fTemp1044) + ((0.018899099999999999 * fTemp1045) + ((0.019806900000000002 * fTemp1046) + ((7.1374800000000001e-06 * fTemp433) + ((5.5996800000000006e-05 * fTemp434) + ((1.69708e-05 * fTemp435) + ((0.000195057 * fTemp436) + ((0.000117308 * fTemp437) + ((0.0143637 * fTemp1047) + ((0.0058000299999999994 * fTemp1048) + ((0.0046881500000000003 * fTemp1049) + ((0.0079024400000000002 * fTemp1050) + ((0.00020216400000000001 * fTemp439) + ((0.0010336799999999999 * fTemp440) + ((0.00042659700000000002 * fTemp441) + ((0.000779071 * fTemp442) + ((0.00070924500000000006 * fTemp443) + ((0.00050421999999999999 * fTemp444) + ((0.00050803600000000001 * fTemp445) + ((0.00095978899999999995 * fTemp446) + ((0.00100501 * fTemp447) + ((0.0012236 * fTemp448) + ((0.00147426 * fTemp449) + ((0.0018419899999999999 * fTemp450) + ((0.00094503799999999993 * fTemp452) + ((0.0013272 * fTemp453) + ((0.0025739000000000001 * fTemp454) + ((0.0033512100000000003 * fTemp455) + ((0.00059498000000000005 * fTemp458) + ((0.00365475 * fTemp459) + ((0.0017505699999999999 * fTemp460) + ((0.0026745600000000003 * fTemp461) + ((0.0010550900000000001 * fTemp1053) + ((0.0046326400000000004 * fTemp463) + ((2.5156400000000002e-05 * fTemp465) + ((0.0053821700000000004 * fTemp466) + ((0.0039087499999999999 * fTemp467) + ((0.0036032700000000004 * fTemp468) + ((0.0019828699999999999 * fTemp469) + ((0.0053022700000000004 * fTemp470) + ((0.00072404000000000001 * fTemp471) + ((0.0033726400000000001 * fTemp473) + ((0.0028127900000000003 * fTemp474) + ((0.0016933400000000002 * fTemp475) + ((0.0029381799999999999 * fTemp476) + ((0.0038801100000000004 * fTemp477) + ((0.0064815100000000002 * fTemp478) + ((0.0067297599999999996 * fTemp479) + ((0.00338193 * fTemp480) + ((0.0026122100000000002 * fTemp481) + ((0.0038020800000000002 * fTemp482) + ((0.0039973999999999999 * fTemp483) + ((4.49754e-06 * fTemp484) + ((0.0042595000000000003 * fTemp485) + ((0.0029229899999999999 * fTemp486) + ((0.00139635 * fTemp487) + ((0.00431154 * fTemp489) + ((0.0105579 * fTemp490) + ((0.014038399999999999 * fTemp491) + ((0.0049862900000000009 * fTemp492) + ((0.0014525200000000001 * fTemp493) + ((0.0072938400000000002 * fTemp494) + ((0.0060665699999999994 * fTemp495) + ((0.0216826 * fTemp496) + ((0.017730600000000003 * fTemp497) + ((0.0123141 * fTemp1055) + ((0.036801199999999999 * fTemp498) + ((0.0075229400000000005 * fTemp499) + ((0.042423700000000002 * fTemp500) + ((0.054464300000000007 * fTemp501) + ((0.111697 * fTemp502) + ((0.050380599999999998 * fTemp503) + ((0.069732299999999997 * fTemp504) + ((0.041405900000000002 * fTemp505) + ((0.047424299999999996 * fTemp506) + ((0.043948600000000004 * fTemp507) + ((0.081345899999999999 * fTemp508) + ((0.062211200000000001 * fTemp509) + ((0.046698700000000003 * fTemp510) + ((0.0034082300000000004 * fTemp1057) + ((0.0017853699999999999 * fTemp1058) + ((0.00345985 * fTemp1059) + ((0.00155444 * fTemp1061) + ((0.000148231 * fTemp517) + ((0.00053192199999999999 * fTemp518) + ((0.0072206400000000004 * fTemp1064) + ((0.0011467599999999999 * fTemp1065) + ((0.0018625499999999999 * fTemp520) + ((0.0014091100000000001 * fTemp521) + ((0.00171149 * fTemp1068) + ((0.0032145400000000001 * fTemp1069) + ((0.0009296310000000001 * fTemp525) + ((0.00017575500000000001 * fTemp526) + ((0.000234556 * fTemp1071) + ((0.0292041 * fTemp529) + ((0.015611 * fTemp530) + ((0.0131959 * fTemp532) + ((0.0033820999999999999 * fTemp533) + ((0.00065433400000000001 * fTemp534) + ((0.00012754700000000002 * fTemp1081) + ((0.00028600400000000002 * fTemp535) + ((0.00056964699999999995 * fTemp536) + ((0.00055874899999999999 * fTemp537) + ((0.000107706 * fTemp538) + ((2.3258800000000005e-05 * fTemp539) + ((1.5002200000000002e-05 * fTemp265) + ((0.000386788 * fTemp1085) + ((0.00090610300000000012 * fTemp541) + ((0.0014508500000000001 * fTemp542) + ((0.0044549899999999998 * fTemp1087) + ((0.00015571700000000001 * fTemp545) + ((0.00054205300000000002 * fTemp1088) + ((0.0015368900000000002 * fTemp1089) + ((0.0026692399999999998 * fTemp546) + ((1.74779e-06 * fTemp547) + ((7.3980599999999995e-07 * fTemp548) + ((0.00144247 * fTemp549) + ((0.0020409600000000001 * fTemp550) + ((0.0020199900000000002 * fTemp551) + ((0.00067166600000000002 * fTemp552) + ((0.0046483699999999998 * fTemp553) + ((0.0007157 * fTemp1090) + ((0.0039002500000000005 * fTemp1091) + ((0.0084629900000000001 * fTemp554) + ((0.053001199999999998 * fTemp555) + ((0.048941400000000003 * fTemp556) + ((0.0007391530000000001 * fTemp557) + ((0.0020685400000000002 * fTemp558) + ((0.00055752700000000004 * fTemp559) + ((0.00107368 * fTemp560) + ((0.00105561 * fTemp561) + ((0.00064681700000000005 * fTemp562) + ((0.00078251799999999995 * fTemp563) + ((0.00084005400000000002 * fTemp564) + ((0.00072445800000000009 * fTemp565) + ((0.00012713900000000002 * fTemp566) + ((1.96889e-05 * fTemp432) + ((5.5660599999999997e-06 * fTemp567) + ((5.6691300000000004e-05 * fTemp1094) + ((0.024367400000000001 * fTemp568) + ((0.011542200000000001 * fTemp1096) + ((0.0023261500000000004 * fTemp570) + ((1.8977800000000003e-05 * fTemp571) + ((1.9873400000000002e-05 * fTemp572) + ((0.00107755 * fTemp1097) + ((1.8648999999999999e-06 * fTemp573) + ((1.4833700000000001e-06 * fTemp1101) + ((0.00016114100000000001 * fTemp575) + ((0.017303499999999999 * fTemp1102) + ((0.0038926099999999999 * fTemp1103) + ((0.0051357799999999995 * fTemp1104) + ((0.0057548899999999995 * fTemp1105) + ((0.0110121 * fTemp1106) + ((0.00038352400000000005 * fTemp576) + ((0.00193882 * fTemp1107) + ((0.0014356600000000001 * fTemp577) + ((0.00020930100000000001 * fTemp578) + ((0.016072400000000001 * fTemp1109) + ((0.0227996 * fTemp1110) + ((0.016965000000000001 * fTemp1111) + ((0.033336499999999998 * fTemp1112) + ((0.029073099999999998 * fTemp1113) + ((0.017693 * fTemp1114) + ((0.023498000000000002 * fTemp1115) + ((0.012867100000000001 * fTemp1116) + ((0.024658199999999998 * fTemp1117) + ((0.033071500000000004 * fTemp1118) + ((0.023601800000000003 * fTemp1119) + ((0.025422899999999998 * fTemp1120) + ((0.030269900000000002 * fTemp1121) + ((0.023295900000000001 * fTemp1122) + ((0.018621700000000001 * fTemp1123) + ((0.010567 * fTemp1124) + ((0.056895100000000004 * fTemp1125) + ((0.014210199999999999 * fTemp1127) + ((0.00027516900000000002 * fTemp1131) + ((0.00128867 * fTemp1132) + ((0.0013097499999999999 * fTemp597) + ((0.0016259900000000001 * fTemp598) + ((0.00050261399999999997 * fTemp599) + ((0.00278079 * fTemp600) + ((0.0081598500000000015 * fTemp602) + ((0.0056564199999999997 * fTemp603) + ((0.010539099999999999 * fTemp604) + ((0.0272114 * fTemp605) + ((0.021074000000000002 * fTemp606) + ((0.0080194200000000011 * fTemp608) + ((0.000377902 * fTemp1133) + ((0.0015717800000000001 * fTemp1136) + ((0.0086705299999999992 * fTemp610) + ((0.00411948 * fTemp1137) + ((0.0029588800000000001 * fTemp612) + ((0.0107584 * fTemp613) + ((0.0093965200000000002 * fTemp614) + ((0.0139764 * fTemp615) + ((0.0010616200000000001 * fTemp1141) + (0.016385799999999999 * fTemp617))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) + (0.00045463200000000001 * fTemp283))))) + (0.042141200000000004 * fTemp279)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) - ((9.8664600000000001e-07 * fTemp620) + ((0.000106835 * fTemp621) + ((3.4608100000000005e-05 * fTemp624) + ((0.00011808300000000001 * fTemp627) + ((0.00015213499999999999 * fTemp629) + ((0.00018018099999999999 * fTemp630) + ((0.00022798800000000001 * fTemp631) + ((5.723480000000001e-05 * fTemp2) + ((0.0010091500000000001 * fTemp632) + ((0.0015510600000000002 * fTemp633) + ((0.00097602600000000006 * fTemp634) + ((0.000712925 * fTemp3) + ((0.00090846500000000001 * fTemp4) + ((0.000274135 * fTemp635) + ((0.0024532300000000003 * fTemp636) + ((0.00327984 * fTemp637) + ((0.00165106 * fTemp638) + ((0.0016773199999999999 * fTemp639) + ((0.00626758 * fTemp640) + ((0.0045976600000000008 * fTemp641) + ((0.0023563100000000003 * fTemp642) + ((0.0048987900000000001 * fTemp643) + ((0.0034805299999999999 * fTemp644) + ((0.0026557099999999999 * fTemp8) + ((0.00044967800000000007 * fTemp645) + ((0.0043297500000000003 * fTemp646) + ((0.009209740000000001 * fTemp647) + ((0.0029224400000000001 * fTemp648) + ((0.0105223 * fTemp9) + ((0.0026383800000000001 * fTemp649) + ((0.0080828600000000007 * fTemp650) + ((0.022465099999999998 * fTemp651) + ((0.0090530699999999999 * fTemp10) + ((0.0048908699999999994 * fTemp652) + ((1.6838600000000002e-05 * fTemp654) + ((0.00013637100000000001 * fTemp13) + ((9.7487500000000005e-05 * fTemp14) + ((2.2899500000000003e-05 * fTemp655) + ((1.3387100000000001e-05 * fTemp656) + ((1.9403399999999997e-06 * fTemp15) + ((0.00034909600000000002 * fTemp16) + ((8.3318600000000019e-05 * fTemp657) + ((0.00019838400000000002 * fTemp18) + ((0.0012272400000000001 * fTemp658) + ((0.00020233 * fTemp21) + ((0.00257691 * fTemp660) + ((0.0010732299999999999 * fTemp661) + ((0.0017717799999999999 * fTemp662) + ((0.000561862 * fTemp626) + ((0.000172719 * fTemp25) + ((0.0028482099999999999 * fTemp663) + ((0.00218125 * fTemp664) + ((0.00186575 * fTemp29) + ((0.000588187 * fTemp31) + ((0.00392757 * fTemp668) + ((0.0028865100000000001 * fTemp669) + ((0.0056144999999999997 * fTemp670) + ((0.0026402700000000001 * fTemp671) + ((0.0042838500000000005 * fTemp33) + ((0.0034927700000000001 * fTemp673) + ((0.0065811400000000009 * fTemp34) + ((0.0015727 * fTemp674) + ((0.00038527200000000003 * fTemp675) + ((0.027060000000000001 * fTemp677) + ((0.0063968899999999997 * fTemp36) + ((0.011650000000000001 * fTemp679) + ((0.0078633899999999996 * fTemp680) + ((0.0092220099999999992 * fTemp681) + ((0.029293300000000001 * fTemp39) + ((6.1935700000000003e-07 * fTemp41) + ((4.9322600000000008e-05 * fTemp682) + ((0.00028692799999999999 * fTemp683) + ((0.00133293 * fTemp44) + ((0.0028975800000000003 * fTemp684) + ((0.00187875 * fTemp685) + ((0.00088707100000000013 * fTemp686) + ((0.00020890300000000003 * fTemp687) + ((0.00237411 * fTemp688) + ((0.00112458 * fTemp689) + ((0.0031641300000000002 * fTemp690) + ((0.0018413699999999999 * fTemp691) + ((0.00090694700000000018 * fTemp55) + ((0.014119 * fTemp692) + ((0.0021459299999999999 * fTemp693) + ((0.015390200000000001 * fTemp58) + ((0.045964499999999998 * fTemp696) + ((0.028787799999999999 * fTemp698) + ((0.048319200000000007 * fTemp699) + ((0.076845299999999991 * fTemp700) + ((0.074437400000000001 * fTemp701) + ((0.0013813599999999999 * fTemp702) + ((9.4869000000000016e-05 * fTemp59) + ((0.00097653599999999996 * fTemp703) + ((0.00011001600000000001 * fTemp60) + ((0.00063438600000000002 * fTemp705) + ((0.00012589200000000001 * fTemp62) + ((0.00040925999999999999 * fTemp706) + ((6.4495100000000005e-05 * fTemp707) + ((1.3287400000000001e-05 * fTemp0) + ((0.00016574 * fTemp708) + ((0.00020847000000000004 * fTemp710) + ((0.00035920700000000001 * fTemp711) + ((0.00031580000000000003 * fTemp712) + ((0.0055766799999999997 * fTemp70) + ((1.6980200000000004e-05 * fTemp713) + ((3.2223700000000001e-05 * fTemp714) + ((0.00013887600000000002 * fTemp715) + ((0.00019982600000000001 * fTemp74) + ((0.00027277099999999999 * fTemp83) + ((0.000964846 * fTemp84) + ((0.0012146699999999999 * fTemp85) + ((0.00057145600000000003 * fTemp86) + ((0.00092367899999999995 * fTemp87) + ((0.0007145500000000001 * fTemp88) + ((0.00031649800000000002 * fTemp720) + ((0.0012203400000000001 * fTemp91) + ((0.0096560400000000012 * fTemp721) + ((0.0144406 * fTemp723) + ((0.013951100000000001 * fTemp724) + ((0.0084374500000000009 * fTemp92) + ((0.0039648399999999999 * fTemp727) + ((0.00036303800000000004 * fTemp95) + ((0.0014542699999999999 * fTemp729) + ((0.00064008700000000001 * fTemp97) + ((7.4982900000000003e-05 * fTemp99) + ((6.501340000000001e-05 * fTemp102) + ((0.00011503900000000001 * fTemp732) + ((7.9125199999999989e-06 * fTemp734) + ((1.1654000000000001e-05 * fTemp735) + ((6.8685100000000004e-05 * fTemp736) + ((8.4929499999999997e-06 * fTemp737) + ((1.41509e-05 * fTemp103) + ((0.00013697000000000001 * fTemp738) + ((0.00010523700000000001 * fTemp739) + ((3.2479200000000006e-05 * fTemp107) + ((7.9775300000000009e-05 * fTemp740) + ((0.00010184400000000001 * fTemp742) + ((5.1085100000000003e-05 * fTemp743) + ((3.2896299999999998e-06 * fTemp114) + ((0.00037059799999999998 * fTemp116) + ((0.000178013 * fTemp117) + ((0.0012454300000000001 * fTemp118) + ((0.0013326899999999999 * fTemp121) + ((0.00140716 * fTemp746) + ((0.00049063800000000005 * fTemp122) + ((0.00084652999999999996 * fTemp748) + ((0.00036433400000000001 * fTemp749) + ((0.001299 * fTemp123) + ((0.00067232300000000006 * fTemp751) + ((0.000145224 * fTemp124) + ((0.0033552399999999998 * fTemp752) + ((0.0029169900000000004 * fTemp753) + ((0.0019041500000000001 * fTemp755) + ((0.0020271300000000003 * fTemp758) + ((0.00118198 * fTemp759) + ((0.0037510500000000001 * fTemp760) + ((0.0044053800000000004 * fTemp761) + ((0.0050914999999999997 * fTemp762) + ((0.0046006600000000003 * fTemp763) + ((0.0069039700000000006 * fTemp764) + ((0.0058685200000000003 * fTemp765) + ((0.0011063700000000002 * fTemp766) + ((0.0019731900000000001 * fTemp130) + ((0.0051318099999999997 * fTemp767) + ((0.0061875000000000003 * fTemp768) + ((0.0086182900000000007 * fTemp769) + ((0.00066500800000000003 * fTemp770) + ((0.0054012200000000008 * fTemp771) + ((0.000641773 * fTemp131) + ((0.0077257100000000002 * fTemp772) + ((0.0049702700000000006 * fTemp773) + ((0.0075730000000000007 * fTemp774) + ((5.7104899999999999e-06 * fTemp134) + ((1.32724e-05 * fTemp135) + ((0.00015924 * fTemp136) + ((3.6292700000000006e-05 * fTemp137) + ((2.5843400000000004e-05 * fTemp777) + ((0.00024267500000000003 * fTemp778) + ((0.00039582199999999999 * fTemp139) + ((0.00031903100000000006 * fTemp140) + ((0.0011941500000000002 * fTemp141) + ((0.00106711 * fTemp142) + ((0.00308618 * fTemp143) + ((0.0032894199999999999 * fTemp144) + ((0.00149796 * fTemp145) + ((0.00068249900000000004 * fTemp146) + ((0.00078510099999999996 * fTemp148) + ((0.00096550100000000012 * fTemp149) + ((0.00058387599999999999 * fTemp150) + ((0.0013118799999999999 * fTemp151) + ((7.6537400000000003e-05 * fTemp152) + ((6.5009300000000008e-05 * fTemp153) + ((0.0031734000000000003 * fTemp154) + ((0.0023903100000000001 * fTemp155) + ((0.00058356200000000003 * fTemp156) + ((1.7606500000000002e-05 * fTemp157) + ((0.0027314400000000003 * fTemp159) + ((0.0017979000000000001 * fTemp160) + ((0.0013939500000000001 * fTemp161) + ((0.00191797 * fTemp162) + ((0.00034747700000000001 * fTemp163) + ((0.0011524899999999999 * fTemp789) + ((0.000320157 * fTemp169) + ((0.0075314300000000004 * fTemp170) + ((0.0020659800000000002 * fTemp172) + ((0.0049689600000000006 * fTemp790) + ((0.00801492 * fTemp791) + ((0.0022648999999999998 * fTemp792) + ((0.0110397 * fTemp173) + ((0.0010152000000000002 * fTemp174) + ((0.00287375 * fTemp794) + ((0.026332300000000003 * fTemp177) + ((0.0161706 * fTemp178) + ((0.0079993000000000009 * fTemp179) + ((0.00082163500000000003 * fTemp797) + ((0.035125400000000001 * fTemp180) + ((0.0032833199999999997 * fTemp182) + ((0.00033950300000000006 * fTemp801) + ((0.00046365600000000006 * fTemp802) + ((0.0019217200000000002 * fTemp184) + ((0.00055710700000000004 * fTemp185) + ((0.00078591400000000008 * fTemp186) + ((0.00067488200000000002 * fTemp187) + ((0.00020588500000000003 * fTemp188) + ((3.4700500000000004e-05 * fTemp189) + ((1.2413700000000001e-05 * fTemp80) + ((0.00022045699999999999 * fTemp805) + ((0.00380796 * fTemp190) + ((3.5918100000000004e-05 * fTemp192) + ((0.0026392099999999999 * fTemp193) + ((5.5551700000000006e-05 * fTemp807) + ((2.5941400000000002e-05 * fTemp194) + ((8.6548500000000007e-05 * fTemp195) + ((4.2405400000000006e-05 * fTemp196) + ((3.5255099999999998e-06 * fTemp197) + ((3.6866300000000003e-05 * fTemp198) + ((4.85157e-05 * fTemp199) + ((7.4248300000000005e-05 * fTemp200) + ((8.1096800000000013e-05 * fTemp201) + ((0.00033669 * fTemp202) + ((0.00035192300000000003 * fTemp203) + ((4.8744899999999996e-07 * fTemp206) + ((3.2621699999999995e-06 * fTemp207) + ((5.5010100000000004e-05 * fTemp809) + ((0.000181368 * fTemp810) + ((0.0012369000000000002 * fTemp211) + ((0.000181872 * fTemp212) + ((0.00084541900000000014 * fTemp813) + ((0.00030825999999999998 * fTemp213) + ((0.000991956 * fTemp214) + ((0.00076287399999999997 * fTemp215) + ((0.00098188600000000017 * fTemp817) + ((0.00039449500000000002 * fTemp216) + ((0.0012472700000000002 * fTemp217) + ((6.1852800000000005e-05 * fTemp218) + ((0.00122741 * fTemp220) + ((0.00019696800000000003 * fTemp822) + ((0.00051085000000000008 * fTemp824) + ((0.0048162999999999999 * fTemp221) + ((0.0040944599999999994 * fTemp223) + ((0.0041798500000000006 * fTemp826) + ((0.0073487099999999996 * fTemp224) + ((0.00164357 * fTemp828) + ((0.0047594300000000003 * fTemp226) + ((0.0066234300000000005 * fTemp227) + ((0.0021625699999999999 * fTemp228) + ((0.0107364 * fTemp229) + ((0.00078494800000000005 * fTemp230) + ((0.008554550000000001 * fTemp231) + ((0.0087573399999999989 * fTemp232) + ((0.0022028300000000002 * fTemp830) + ((0.026865800000000002 * fTemp233) + ((0.0066563000000000004 * fTemp833) + ((4.2778300000000003e-05 * fTemp235) + ((9.4569700000000004e-05 * fTemp834) + ((0.000158107 * fTemp835) + ((0.00048340500000000007 * fTemp836) + ((0.00023814000000000002 * fTemp837) + ((0.010395300000000001 * fTemp840) + ((0.0093701299999999991 * fTemp841) + ((0.0063169100000000002 * fTemp842) + ((0.0060908100000000003 * fTemp843) + ((0.013098200000000001 * fTemp844) + ((0.0095365099999999998 * fTemp845) + ((4.9636399999999996e-06 * fTemp846) + ((0.000168486 * fTemp847) + ((0.00017177199999999999 * fTemp848) + ((0.0011732700000000001 * fTemp851) + ((0.00090841200000000004 * fTemp239) + ((0.0016132899999999999 * fTemp240) + ((0.00080033999999999997 * fTemp243) + ((0.00113243 * fTemp245) + ((2.5690700000000001e-05 * fTemp246) + ((0.0016462200000000001 * fTemp852) + ((0.0158301 * fTemp853) + ((0.0126207 * fTemp254) + ((0.015590100000000001 * fTemp855) + ((0.027497799999999999 * fTemp857) + ((0.019302299999999998 * fTemp858) + ((0.044481599999999996 * fTemp859) + ((0.042276800000000003 * fTemp860) + ((0.0021177600000000002 * fTemp861) + ((0.00027291300000000001 * fTemp862) + ((6.1550000000000005e-05 * fTemp256) + ((0.000175224 * fTemp864) + ((0.00022650199999999998 * fTemp865) + ((5.7022700000000005e-05 * fTemp866) + ((2.0383200000000002e-05 * fTemp867) + ((4.9413399999999999e-06 * fTemp71) + ((0.00042647700000000008 * fTemp261) + ((0.0042857099999999999 * fTemp262) + ((0.0037509800000000001 * fTemp263) + ((0.000469171 * fTemp264) + ((0.0013037700000000001 * fTemp267) + ((6.2934400000000005e-05 * fTemp877) + ((0.000195522 * fTemp878) + ((0.00010142800000000001 * fTemp879) + ((0.00070563799999999997 * fTemp880) + ((0.00031068799999999999 * fTemp271) + ((0.0015255399999999999 * fTemp881) + ((0.000380057 * fTemp882) + ((0.00093046600000000002 * fTemp619) + ((7.7027e-06 * fTemp884) + ((5.28142e-05 * fTemp885) + ((0.00019374600000000001 * fTemp281) + ((0.00024288000000000001 * fTemp282) + ((0.00086436100000000012 * fTemp284) + ((5.3978799999999999e-06 * fTemp888) + ((5.8471100000000003e-05 * fTemp889) + ((0.00011763 * fTemp890) + ((0.00096584900000000007 * fTemp891) + ((0.000544179 * fTemp285) + ((0.00077855999999999997 * fTemp287) + ((0.00043610699999999998 * fTemp289) + ((0.000204336 * fTemp290) + ((2.2571e-06 * fTemp291) + ((1.2883700000000002e-05 * fTemp191) + ((0.0065755600000000003 * fTemp901) + ((0.044476699999999994 * fTemp292) + ((0.019425600000000001 * fTemp293) + ((0.0258357 * fTemp295) + ((0.00505303 * fTemp296) + ((0.00065851199999999999 * fTemp297) + ((6.4017400000000008e-05 * fTemp908) + ((0.00033249800000000003 * fTemp298) + ((0.00114682 * fTemp299) + ((0.0173968 * fTemp302) + ((0.00029377599999999999 * fTemp913) + ((0.000280439 * fTemp914) + ((0.000643591 * fTemp916) + ((0.00081294000000000006 * fTemp917) + ((0.00023107600000000004 * fTemp918) + ((0.00064061300000000005 * fTemp919) + ((0.00091664000000000003 * fTemp920) + ((0.00059890600000000007 * fTemp921) + ((0.00073955600000000005 * fTemp922) + ((0.0027768799999999998 * fTemp923) + ((0.0022861500000000002 * fTemp925) + ((0.0013073099999999999 * fTemp926) + ((0.00145302 * fTemp310) + ((0.00040949500000000001 * fTemp927) + ((0.00082950500000000002 * fTemp928) + ((0.00070229999999999999 * fTemp929) + ((0.00080632699999999991 * fTemp930) + ((0.001377 * fTemp312) + ((0.00016274 * fTemp316) + ((0.0028667500000000004 * fTemp932) + ((0.0011399600000000002 * fTemp933) + ((0.0027307400000000002 * fTemp934) + ((0.00100318 * fTemp935) + ((0.0022987899999999998 * fTemp936) + ((0.00110995 * fTemp937) + ((0.00124602 * fTemp938) + ((0.00129805 * fTemp939) + ((0.0023355800000000003 * fTemp940) + ((0.0019049100000000001 * fTemp941) + ((0.00050953999999999995 * fTemp942) + ((0.0038312400000000001 * fTemp943) + ((0.0011070699999999999 * fTemp944) + ((0.00059075000000000002 * fTemp945) + ((0.00044511000000000003 * fTemp322) + ((0.0016638 * fTemp948) + ((0.0043739400000000006 * fTemp330) + ((0.0106382 * fTemp949) + ((0.0043469900000000002 * fTemp950) + ((0.0015759000000000001 * fTemp333) + ((0.0073185999999999998 * fTemp334) + ((0.0045973800000000007 * fTemp952) + ((0.013183400000000001 * fTemp335) + ((0.028645500000000001 * fTemp953) + ((0.0092487699999999999 * fTemp338) + ((0.039121700000000002 * fTemp954) + ((0.00144425 * fTemp955) + ((0.0060285 * fTemp956) + ((0.057524800000000001 * fTemp957) + ((0.0013932899999999999 * fTemp342) + ((7.8904099999999995e-07 * fTemp349) + ((3.48346e-05 * fTemp350) + ((5.1851200000000009e-05 * fTemp351) + ((0.000263489 * fTemp962) + ((0.00072508600000000011 * fTemp963) + ((0.00110553 * fTemp964) + ((0.0018638400000000001 * fTemp965) + ((0.00082826600000000003 * fTemp966) + ((0.0013655500000000001 * fTemp967) + ((0.0013615999999999999 * fTemp968) + ((0.00103053 * fTemp969) + ((0.00040302800000000005 * fTemp353) + ((0.0012814900000000001 * fTemp354) + ((0.0015273399999999999 * fTemp970) + ((0.00046550599999999997 * fTemp971) + ((0.00038473700000000001 * fTemp355) + ((0.0010250300000000001 * fTemp356) + ((0.0011346700000000002 * fTemp358) + ((0.00043984100000000006 * fTemp363) + ((0.0016748900000000001 * fTemp973) + ((0.00097958799999999999 * fTemp974) + ((0.00066577900000000002 * fTemp975) + ((0.00120272 * fTemp976) + ((0.0010960700000000002 * fTemp364) + ((0.0013700399999999999 * fTemp978) + ((0.0019637600000000002 * fTemp980) + ((0.0018260300000000002 * fTemp981) + ((0.00238017 * fTemp982) + ((0.0030635300000000001 * fTemp983) + ((0.00021673800000000001 * fTemp367) + ((0.0025982900000000001 * fTemp984) + ((0.0016457800000000001 * fTemp985) + ((0.0013408299999999999 * fTemp986) + ((0.0031654200000000004 * fTemp988) + ((0.00275033 * fTemp989) + ((0.00152893 * fTemp370) + ((0.00068048499999999999 * fTemp371) + ((0.0022063899999999999 * fTemp990) + ((0.00122218 * fTemp372) + ((0.00078379700000000003 * fTemp992) + ((0.00022987799999999998 * fTemp375) + ((0.00078511300000000004 * fTemp380) + ((0.0012260999999999999 * fTemp381) + ((0.0026344099999999998 * fTemp384) + ((0.0038961999999999998 * fTemp386) + ((0.011467400000000001 * fTemp995) + ((0.0078678299999999993 * fTemp996) + ((0.0089890100000000004 * fTemp391) + ((0.0013363100000000001 * fTemp997) + ((0.0143689 * fTemp998) + ((0.0049334100000000001 * fTemp999) + ((0.019686699999999998 * fTemp1000) + ((0.0086390499999999988 * fTemp1001) + ((0.043355600000000001 * fTemp1004) + ((0.00249322 * fTemp1005) + ((0.0141169 * fTemp1006) + ((0.0111431 * fTemp1007) + ((0.0018486100000000001 * fTemp399) + ((0.000976993 * fTemp400) + ((0.0056459800000000001 * fTemp1008) + ((0.00021246900000000004 * fTemp1009) + ((0.00095016999999999996 * fTemp1010) + ((0.0010622000000000001 * fTemp1012) + ((0.00024919200000000002 * fTemp1013) + ((0.00027049600000000001 * fTemp1014) + ((5.1123399999999996e-06 * fTemp404) + ((3.7580000000000003e-05 * fTemp405) + ((1.0426200000000002e-05 * fTemp344) + ((2.5019199999999998e-06 * fTemp1018) + ((0.00041578400000000004 * fTemp407) + ((2.3801800000000003e-05 * fTemp1031) + ((0.00042180200000000004 * fTemp408) + ((0.00064933700000000005 * fTemp409) + ((0.00040693400000000007 * fTemp410) + ((0.00073777800000000002 * fTemp411) + ((0.0013956400000000001 * fTemp412) + ((0.0012716300000000002 * fTemp413) + ((0.0010572399999999999 * fTemp415) + ((0.00096599000000000012 * fTemp416) + ((0.00161012 * fTemp417) + ((0.0016711600000000001 * fTemp418) + ((0.00075290600000000002 * fTemp419) + ((0.00144013 * fTemp420) + ((0.00065036 * fTemp421) + ((0.0021273399999999997 * fTemp422) + ((0.00061597300000000002 * fTemp423) + ((0.0020596199999999999 * fTemp424) + ((0.00011313 * fTemp1033) + ((0.0017438199999999999 * fTemp425) + ((0.0024799100000000001 * fTemp426) + ((0.00130908 * fTemp1034) + ((0.00069698000000000004 * fTemp429) + ((0.0017169000000000002 * fTemp431) + ((0.0011552999999999999 * fTemp1036) + ((1.47262e-06 * fTemp438) + ((0.00048385000000000002 * fTemp451) + ((0.0023706000000000001 * fTemp456) + ((9.940880000000001e-05 * fTemp457) + ((0.00196204 * fTemp1051) + ((1.7742000000000001e-05 * fTemp1052) + ((0.000611186 * fTemp462) + ((0.00047365100000000001 * fTemp1054) + ((0.000687957 * fTemp464) + ((0.00036222200000000004 * fTemp472) + ((0.00092078700000000006 * fTemp488) + ((0.021295600000000001 * fTemp1056) + ((0.0025833200000000001 * fTemp511) + ((0.00054242299999999999 * fTemp512) + ((0.00061493000000000008 * fTemp513) + ((2.9137400000000003e-05 * fTemp514) + ((0.0015830800000000002 * fTemp515) + ((0.0017704200000000002 * fTemp1060) + ((0.0019330199999999999 * fTemp516) + ((0.00156606 * fTemp1062) + ((0.00266146 * fTemp1063) + ((0.0011897800000000001 * fTemp519) + ((0.00307881 * fTemp522) + ((0.0024114100000000001 * fTemp1066) + ((6.3317800000000006e-05 * fTemp523) + ((0.00026003500000000004 * fTemp1067) + ((4.06072e-05 * fTemp524) + ((0.000175864 * fTemp1070) + ((0.000162845 * fTemp527) + ((0.00103864 * fTemp1072) + ((0.0013787300000000001 * fTemp1073) + ((0.00181733 * fTemp1074) + ((0.00293582 * fTemp1075) + ((0.0014793400000000002 * fTemp528) + ((0.021570599999999999 * fTemp1076) + ((0.0043988899999999999 * fTemp1077) + ((0.0073680199999999994 * fTemp531) + ((0.00058021900000000009 * fTemp1078) + ((0.0037306700000000002 * fTemp1079) + ((0.000216524 * fTemp1080) + ((0.00050577300000000005 * fTemp1082) + ((0.00019702400000000002 * fTemp1083) + ((6.4290300000000003e-05 * fTemp1084) + ((0.00256507 * fTemp540) + ((0.000597807 * fTemp543) + ((2.74177e-05 * fTemp1086) + ((0.0018893200000000001 * fTemp544) + ((0.00016075400000000001 * fTemp1092) + ((0.00052814500000000005 * fTemp1093) + ((0.0017080400000000001 * fTemp1095) + ((7.4329200000000013e-05 * fTemp569) + ((0.00048678400000000003 * fTemp1098) + ((0.00094771800000000006 * fTemp1099) + ((0.00085589900000000003 * fTemp1100) + ((0.00015476800000000001 * fTemp574) + ((0.00087519700000000008 * fTemp1108) + ((0.00092920600000000002 * fTemp1126) + ((0.080136100000000002 * fTemp579) + ((0.0088175100000000006 * fTemp580) + ((0.042661300000000006 * fTemp581) + ((0.041393800000000001 * fTemp582) + ((0.097787600000000002 * fTemp583) + ((0.053196100000000003 * fTemp584) + ((0.040092800000000005 * fTemp585) + ((0.077296500000000004 * fTemp586) + ((0.082935599999999998 * fTemp587) + ((0.064767000000000005 * fTemp588) + ((0.00067357800000000009 * fTemp589) + ((0.078489900000000001 * fTemp590) + ((0.00149621 * fTemp1128) + ((0.00072128000000000003 * fTemp591) + ((0.00101019 * fTemp1129) + ((0.000453602 * fTemp592) + ((0.00042969900000000002 * fTemp1130) + ((0.0010723199999999999 * fTemp593) + ((0.00084950300000000004 * fTemp594) + ((0.00085153200000000001 * fTemp595) + ((0.00015027000000000001 * fTemp596) + ((2.4709800000000002e-05 * fTemp286) + ((0.00063317100000000008 * fTemp601) + ((0.0029395300000000001 * fTemp607) + ((0.00025527000000000004 * fTemp618) + ((0.00098456699999999999 * fTemp1134) + ((0.0060422599999999998 * fTemp1135) + ((0.0027149600000000002 * fTemp609) + ((5.1858400000000005e-05 * fTemp611) + ((0.00081970899999999995 * fTemp1138) + ((0.0045739400000000003 * fTemp1139) + ((0.00060354400000000002 * fTemp1140) + ((5.63033e-05 * fTemp616) + (0.0033351100000000001 * fTemp1142)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
			fRec12[0] = std::max<double>((fRec12[1] - fConst0), std::min<double>(6.0, (20.0 * std::log10(std::max<double>(0.00031622776601683794, std::fabs(fTemp1144))))));
			fHbargraph1 = FAUSTFLOAT(fRec12[0]);
			output1[i] = FAUSTFLOAT(fTemp1144);
			fRec1[1] = fRec1[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			IOTA = (IOTA + 1);
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec0[1] = fRec0[0];
			fRec12[1] = fRec12[0];
		}
	}

};

#ifdef FAUST_UIMACROS
	
	#define FAUST_FILE_NAME "ue.binaural.decoder.dsp"
	#define FAUST_CLASS_NAME "ue_binaural_decoder"
	#define FAUST_INPUTS 9
	#define FAUST_OUTPUTS 2
	#define FAUST_ACTIVES 14
	#define FAUST_PASSIVES 11

	FAUST_ADDCHECKBOX("Inputs/0/ 0/Mute", fCheckbox11);
	FAUST_ADDVERTICALBARGRAPH("Inputs/0/ 0/", fVbargraph8, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/0/Mute", fCheckbox10);
	FAUST_ADDCHECKBOX("Inputs/1/ 1/Mute", fCheckbox8);
	FAUST_ADDVERTICALBARGRAPH("Inputs/1/ 1/", fVbargraph6, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/1/ 2/Mute", fCheckbox9);
	FAUST_ADDVERTICALBARGRAPH("Inputs/1/ 2/", fVbargraph7, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/1/ 3/Mute", fCheckbox7);
	FAUST_ADDVERTICALBARGRAPH("Inputs/1/ 3/", fVbargraph5, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/1/Mute", fCheckbox6);
	FAUST_ADDCHECKBOX("Inputs/2/ 4/Mute", fCheckbox5);
	FAUST_ADDVERTICALBARGRAPH("Inputs/2/ 4/", fVbargraph4, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/2/ 5/Mute", fCheckbox4);
	FAUST_ADDVERTICALBARGRAPH("Inputs/2/ 5/", fVbargraph3, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/2/ 6/Mute", fCheckbox3);
	FAUST_ADDVERTICALBARGRAPH("Inputs/2/ 6/", fVbargraph2, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/2/ 7/Mute", fCheckbox2);
	FAUST_ADDVERTICALBARGRAPH("Inputs/2/ 7/", fVbargraph1, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/2/ 8/Mute", fCheckbox1);
	FAUST_ADDVERTICALBARGRAPH("Inputs/2/ 8/", fVbargraph0, -70.0, 6.0);
	FAUST_ADDCHECKBOX("Inputs/2/Mute", fCheckbox0);
	FAUST_ADDVERTICALSLIDER("Inputs/Inputs Gain", fVslider1, 0.0, -10.0, 10.0, 0.10000000000000001);
	FAUST_ADDVERTICALSLIDER("Inputs/Outputs Gain", fVslider0, 0.0, -10.0, 10.0, 0.10000000000000001);
	FAUST_ADDHORIZONTALBARGRAPH("Outputs/Left/", fHbargraph0, -70.0, 6.0);
	FAUST_ADDHORIZONTALBARGRAPH("Outputs/Right/", fHbargraph1, -70.0, 6.0);

	#define FAUST_LIST_ACTIVES(p) \
		p(CHECKBOX, Mute, "Inputs/0/ 0/Mute", fCheckbox11, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/0/Mute", fCheckbox10, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/1/ 1/Mute", fCheckbox8, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/1/ 2/Mute", fCheckbox9, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/1/ 3/Mute", fCheckbox7, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/1/Mute", fCheckbox6, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/ 4/Mute", fCheckbox5, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/ 5/Mute", fCheckbox4, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/ 6/Mute", fCheckbox3, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/ 7/Mute", fCheckbox2, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/ 8/Mute", fCheckbox1, 0.0, 0.0, 1.0, 1.0) \
		p(CHECKBOX, Mute, "Inputs/2/Mute", fCheckbox0, 0.0, 0.0, 1.0, 1.0) \
		p(VERTICALSLIDER, Inputs_Gain, "Inputs/Inputs Gain", fVslider1, 0.0, -10.0, 10.0, 0.10000000000000001) \
		p(VERTICALSLIDER, Outputs_Gain, "Inputs/Outputs Gain", fVslider0, 0.0, -10.0, 10.0, 0.10000000000000001) \

	#define FAUST_LIST_PASSIVES(p) \
		p(VERTICALBARGRAPH, , "Inputs/0/ 0/", fVbargraph8, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/1/ 1/", fVbargraph6, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/1/ 2/", fVbargraph7, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/1/ 3/", fVbargraph5, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/2/ 4/", fVbargraph4, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/2/ 5/", fVbargraph3, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/2/ 6/", fVbargraph2, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/2/ 7/", fVbargraph1, 0.0, -70.0, 6.0, 0.0) \
		p(VERTICALBARGRAPH, , "Inputs/2/ 8/", fVbargraph0, 0.0, -70.0, 6.0, 0.0) \
		p(HORIZONTALBARGRAPH, , "Outputs/Left/", fHbargraph0, 0.0, -70.0, 6.0, 0.0) \
		p(HORIZONTALBARGRAPH, , "Outputs/Right/", fHbargraph1, 0.0, -70.0, 6.0, 0.0) \

#endif

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

/* Faust code wrapper ------- */

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "jpatcher_api.h"
#include <string.h>

#define ASSIST_INLET 	1
#define ASSIST_OUTLET 	2

#define EXTERNAL_VERSION    "0.77"
#define STR_SIZE            512

/************************** BEGIN MidiUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MIDIUI_H
#define FAUST_MIDIUI_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <cmath>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>


/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        { 
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            while (index-- > 0 && it++ != fPathZoneMap.end()) {}
            return (*it).first;
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            std::map<std::string, FAUSTFLOAT*>::iterator it = fPathZoneMap.begin();
            do {
                if ((*it).second == zone) return (*it).first;
            }
            while (it++ != fPathZoneMap.end());
            return "";
        }
    
        static bool endsWith(std::string const& str, std::string const& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN midi.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __midi__
#define __midi__

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

class MapUI;

/*************************************
 A time-stamped short MIDI message
**************************************/

// Force contiguous memory layout
#pragma pack (push, 1)
struct MIDIMessage
{
    uint32_t frameIndex;
    uint8_t byte0, byte1, byte2;
};
#pragma pack (pop)

/*******************************************************************************
 * MIDI processor definition.
 *
 * MIDI input or output handling classes will implement this interface,
 * so the same method names (keyOn, ctrlChange...) will be used either
 * when decoding MIDI input or encoding MIDI output events.
 *******************************************************************************/

class midi {

    public:

        midi() {}
        virtual ~midi() {}

        // Additional time-stamped API for MIDI input
        virtual MapUI* keyOn(double, int channel, int pitch, int velocity)
        {
            return keyOn(channel, pitch, velocity);
        }
        
        virtual void keyOff(double, int channel, int pitch, int velocity = 127)
        {
            keyOff(channel, pitch, velocity);
        }
    
        virtual void keyPress(double, int channel, int pitch, int press)
        {
            keyPress(channel, pitch, press);
        }
        
        virtual void chanPress(double date, int channel, int press)
        {
            chanPress(channel, press);
        }
    
        virtual void pitchWheel(double, int channel, int wheel)
        {
            pitchWheel(channel, wheel);
        }
           
        virtual void ctrlChange(double, int channel, int ctrl, int value)
        {
            ctrlChange(channel, ctrl, value);
        }
    
        virtual void ctrlChange14bits(double, int channel, int ctrl, int value)
        {
            ctrlChange14bits(channel, ctrl, value);
        }
    
        virtual void rpn(double, int channel, int ctrl, int value)
        {
            rpn(channel, ctrl, value);
        }

        virtual void progChange(double, int channel, int pgm)
        {
            progChange(channel, pgm);
        }
    
        virtual void sysEx(double, std::vector<unsigned char>& message)
        {
            sysEx(message);
        }

        // MIDI sync
        virtual void startSync(double date)  {}
        virtual void stopSync(double date)   {}
        virtual void clock(double date)  {}

        // Standard MIDI API
        virtual MapUI* keyOn(int channel, int pitch, int velocity)      { return nullptr; }
        virtual void keyOff(int channel, int pitch, int velocity)       {}
        virtual void keyPress(int channel, int pitch, int press)        {}
        virtual void chanPress(int channel, int press)                  {}
        virtual void ctrlChange(int channel, int ctrl, int value)       {}
        virtual void ctrlChange14bits(int channel, int ctrl, int value) {}
        virtual void rpn(int channel, int ctrl, int value)              {}
        virtual void pitchWheel(int channel, int wheel)                 {}
        virtual void progChange(int channel, int pgm)                   {}
        virtual void sysEx(std::vector<unsigned char>& message)         {}

        enum MidiStatus {
            // channel voice messages
            MIDI_NOTE_OFF = 0x80,
            MIDI_NOTE_ON = 0x90,
            MIDI_CONTROL_CHANGE = 0xB0,
            MIDI_PROGRAM_CHANGE = 0xC0,
            MIDI_PITCH_BEND = 0xE0,
            MIDI_AFTERTOUCH = 0xD0,         // aka channel pressure
            MIDI_POLY_AFTERTOUCH = 0xA0,    // aka key pressure
            MIDI_CLOCK = 0xF8,
            MIDI_START = 0xFA,
            MIDI_CONT = 0xFB,
            MIDI_STOP = 0xFC,
            MIDI_SYSEX_START = 0xF0,
            MIDI_SYSEX_STOP = 0xF7
        };

        enum MidiCtrl {
            ALL_NOTES_OFF = 123,
            ALL_SOUND_OFF = 120
        };
    
        enum MidiNPN {
            PITCH_BEND_RANGE = 0
        };

};

/*
 A class to decode NRPN and RPN messages, adapted from JUCE forum message: https://forum.juce.com/t/14bit-midi-controller-support/11517
*/

class MidiNRPN {
    
    private:
    
        bool ctrlnew;
        int ctrlnum;
        int ctrlval;
        
        int nrpn_lsb, nrpn_msb;
        int data_lsb, data_msb;
        
        enum
        {
            midi_nrpn_lsb = 98,
            midi_nrpn_msb = 99,
            midi_rpn_lsb  = 100,
            midi_rpn_msb  = 101,
            midi_data_lsb = 38,
            midi_data_msb = 6
        };
    
    public:
        
        MidiNRPN(): ctrlnew(false), nrpn_lsb(-1), nrpn_msb(-1), data_lsb(-1), data_msb(-1)
        {}
        
        // return true if the message has been filtered
        bool process(int data1, int data2)
        {
            switch (data1)
            {
                case midi_nrpn_lsb: nrpn_lsb = data2; return true;
                case midi_nrpn_msb: nrpn_msb = data2; return true;
                case midi_rpn_lsb: {
                    if (data2 == 127) {
                        nrpn_lsb = data_lsb = -1;
                    } else {
                        nrpn_lsb = 0;
                        data_lsb = -1;
                    }
                    return true;
                }
                case midi_rpn_msb: {
                    if (data2 == 127) {
                        nrpn_msb = data_msb = -1;
                    } else {
                        nrpn_msb = 0;
                        data_msb = -1;
                    }
                    return true;
                }
                case midi_data_lsb:
                case midi_data_msb:
                {
                    if (data1 == midi_data_msb) {
                        if (nrpn_msb < 0) {
                            return false;
                        }
                        data_msb = data2;
                    } else { // midi_data_lsb
                        if (nrpn_lsb < 0) {
                            return false;
                        }
                        data_lsb = data2;
                    }
                    if (data_lsb >= 0 && data_msb >= 0) {
                        ctrlnum = (nrpn_msb << 7) | nrpn_lsb;
                        ctrlval = (data_msb << 7) | data_lsb;
                        data_lsb = data_msb = -1;
                        nrpn_msb = nrpn_lsb = -1;
                        ctrlnew = true;
                    }
                    return true;
                }
                default: return false;
            };
        }
        
        bool hasNewNRPN() { bool res = ctrlnew; ctrlnew = false; return res; }
        
        // results in [0, 16383]
        int getCtrl() const { return ctrlnum; }
        int getVal() const { return ctrlval; }
    
};


/****************************************************
 * Base class for MIDI input handling.
 *
 * Shared common code used for input handling:
 * - decoding Real-Time messages: handleSync
 * - decoding one data byte messages: handleData1
 * - decoding two data byte messages: handleData2
 * - getting ready messages in polling mode
 ****************************************************/

class midi_handler : public midi {

    protected:

        std::vector<midi*> fMidiInputs;
        std::string fName;
        MidiNRPN fNRPN;
    
        int range(int min, int max, int val) { return (val < min) ? min : ((val >= max) ? max : val); }
  
    public:

        midi_handler(const std::string& name = "MIDIHandler"):fName(name) {}
        virtual ~midi_handler() {}

        void addMidiIn(midi* midi_dsp) { if (midi_dsp) fMidiInputs.push_back(midi_dsp); }
        void removeMidiIn(midi* midi_dsp)
        {
            std::vector<midi*>::iterator it = std::find(fMidiInputs.begin(), fMidiInputs.end(), midi_dsp);
            if (it != fMidiInputs.end()) {
                fMidiInputs.erase(it);
            }
        }

        // Those 2 methods have to be implemented by subclasses
        virtual bool startMidi() { return true; }
        virtual void stopMidi() {}
    
        void setName(const std::string& name) { fName = name; }
        std::string getName() { return fName; }
    
        // To be used in polling mode
        virtual int recvMessages(std::vector<MIDIMessage>* message) { return 0; }
        virtual void sendMessages(std::vector<MIDIMessage>* message, int count) {}
    
        // MIDI Real-Time
        void handleClock(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->clock(time);
            }
        }
        
        void handleStart(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->startSync(time);
            }
        }
        
        void handleStop(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->stopSync(time);
            }
        }
    
        void handleSync(double time, int type)
        {
            if (type == MIDI_CLOCK) {
                handleClock(time);
            // We can consider start and continue as identical messages
            } else if ((type == MIDI_START) || (type == MIDI_CONT)) {
                handleStart(time);
            } else if (type == MIDI_STOP) {
                handleStop(time);
            }
        }
    
        // MIDI 1 data
        void handleProgChange(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->progChange(time, channel, data1);
            }
        }
    
        void handleAfterTouch(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->chanPress(time, channel, data1);
            }
        }

        void handleData1(double time, int type, int channel, int data1)
        {
            if (type == MIDI_PROGRAM_CHANGE) {
                handleProgChange(time, channel, data1);
            } else if (type == MIDI_AFTERTOUCH) {
                handleAfterTouch(time, channel, data1);
            }
        }
    
        // MIDI 2 datas
        void handleKeyOff(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyOff(time, channel, data1, data2);
            }
        }
        
        void handleKeyOn(double time, int channel, int data1, int data2)
        {
            if (data2 == 0) {
                handleKeyOff(time, channel, data1, data2);
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->keyOn(time, channel, data1, data2);
                }
            }
        }
    
        void handleCtrlChange(double time, int channel, int data1, int data2)
        {
            // Special processing for NRPN and RPN
            if (fNRPN.process(data1, data2)) {
                if (fNRPN.hasNewNRPN()) {
                    for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                        fMidiInputs[i]->rpn(time, channel, fNRPN.getCtrl(), fNRPN.getVal());
                    }
                }
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->ctrlChange(time, channel, data1, data2);
                }
            }
        }
        
        void handlePitchWheel(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->pitchWheel(time, channel, (data2 << 7) + data1);
            }
        }
    
        void handlePitchWheel(double time, int channel, int bend)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->pitchWheel(time, channel, bend);
            }
        }
        
        void handlePolyAfterTouch(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyPress(time, channel, data1, data2);
            }
        }
  
        void handleData2(double time, int type, int channel, int data1, int data2)
        {
            if (type == MIDI_NOTE_OFF) {
                handleKeyOff(time, channel,  data1, data2);
            } else if (type == MIDI_NOTE_ON) {
                handleKeyOn(time, channel, data1, data2);
            } else if (type == MIDI_CONTROL_CHANGE) {
                handleCtrlChange(time, channel, data1, data2);
            } else if (type == MIDI_PITCH_BEND) {
                handlePitchWheel(time, channel, data1, data2);
            } else if (type == MIDI_POLY_AFTERTOUCH) {
                handlePolyAfterTouch(time, channel, data1, data2);
            }
        }
    
        // SysEx
        void handleSysex(double time, std::vector<unsigned char>& message)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->sysEx(time, message);
            }
        }
    
        void handleMessage(double time, int type, std::vector<unsigned char>& message)
        {
            if (type == MIDI_SYSEX_START) {
                handleSysex(time, message);
            }
        }
  
};

//-------------------------------
// For timestamped MIDI messages
//-------------------------------

struct DatedMessage {
    
    double fDate;
    unsigned char fBuffer[3];
    size_t fSize;
    
    DatedMessage(double date, unsigned char* buffer, size_t size)
    :fDate(date), fSize(size)
    {
        assert(size <= 3);
        memcpy(fBuffer, buffer, size);
    }
    
    DatedMessage():fDate(0.0), fSize(0)
    {}
    
};

#endif // __midi__
/**************************  END  midi.h **************************/

#ifdef _MSC_VER
#define gsscanf sscanf_s
#else
#define gsscanf sscanf
#endif

/*****************************************************************************
* Helper code for MIDI meta and polyphonic 'nvoices' parsing
******************************************************************************/

struct MidiMeta : public Meta, public std::map<std::string, std::string> {
    
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
    
    const std::string get(const char* key, const char* def)
    {
        return (this->find(key) != this->end()) ? (*this)[key] : def;
    }
    
    static void analyse(dsp* mono_dsp, bool& midi_sync, int& nvoices)
    {
        JSONUI jsonui;
        mono_dsp->buildUserInterface(&jsonui);
        std::string json = jsonui.JSON();
        midi_sync = ((json.find("midi") != std::string::npos) &&
                     ((json.find("start") != std::string::npos) ||
                      (json.find("stop") != std::string::npos) ||
                      (json.find("clock") != std::string::npos) ||
                      (json.find("timestamp") != std::string::npos)));
    
    #if defined(NVOICES) && NVOICES!=NUM_VOICES
        nvoices = NVOICES;
    #else
        MidiMeta meta;
        mono_dsp->metadata(&meta);
        bool found_voices = false;
        // If "options" metadata is used
        std::string options = meta.get("options", "");
        if (options != "") {
            std::map<std::string, std::string> metadata;
            std::string res;
            MetaDataUI::extractMetadata(options, res, metadata);
            if (metadata.find("nvoices") != metadata.end()) {
                nvoices = std::atoi(metadata["nvoices"].c_str());
                found_voices = true;
            }
        }
        // Otherwise test for "nvoices" metadata
        if (!found_voices) {
            std::string numVoices = meta.get("nvoices", "0");
            nvoices = std::atoi(numVoices.c_str());
        }
        nvoices = std::max<int>(0, nvoices);
    #endif
    }
    
    static bool checkPolyphony(dsp* mono_dsp)
    {
        MapUI map_ui;
        mono_dsp->buildUserInterface(&map_ui);
        bool has_freq = false;
        bool has_gate = false;
        bool has_gain = false;
        for (int i = 0; i < map_ui.getParamsCount(); i++) {
            std::string path = map_ui.getParamAddress(i);
            has_freq |= MapUI::endsWith(path, "/freq");
            has_gate |= MapUI::endsWith(path, "/gate");
            has_gain |= MapUI::endsWith(path, "/gain");
        }
        return (has_freq && has_gate && has_gain);
    }
    
};

/*******************************************************************************
 * MidiUI : Faust User Interface
 * This class decodes MIDI meta data and maps incoming MIDI messages to them.
 * Currently ctrl, keyon/keyoff, keypress, pgm, chanpress, pitchwheel/pitchbend
 * start/stop/clock meta data is handled.
 ******************************************************************************/

class uiMidi {
    
    friend class MidiUI;
    
    protected:
        
        midi* fMidiOut;
        bool fInputCtrl;
        int fChan;
    
        // To be used when sending messages, returns the effective chan, or 0 when fChan is initialized with -1 (means 'all chans')
        int rangeChan() { return (((fChan < 0) || (fChan > 15)) ? 0 : fChan); }
        bool inRange(FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT v) { return (min <= v && v <= max); }
    
    public:
        
        uiMidi(midi* midi_out, bool input, int chan = -1):fMidiOut(midi_out), fInputCtrl(input), fChan(chan)
        {}
        virtual ~uiMidi()
        {}

};

/*****************************************************************************
 * Base class for MIDI aware UI items
 ******************************************************************************/

class uiMidiItem : public uiMidi, public uiItem {
    
    public:
        
        uiMidiItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidi(midi_out, input, chan), uiItem(ui, zone)
        {}
        virtual ~uiMidiItem()
        {}
    
        virtual void reflectZone() {}
    
};

/*****************************************************************************
 * Base class for MIDI aware UI items with timestamp support
 ******************************************************************************/

class uiMidiTimedItem : public uiMidi, public uiTimedItem {
    
    public:
        
        uiMidiTimedItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = -1)
            :uiMidi(midi_out, input, chan), uiTimedItem(ui, zone)
        {}
        virtual ~uiMidiTimedItem()
        {}
    
        virtual void reflectZone() {}
    
};

//-----------
// MIDI sync
//-----------

class uiMidiStart : public uiMidiTimedItem
{
  
    public:
    
        uiMidiStart(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStart()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->startSync(0);
            }
        }
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
        
};

class uiMidiStop : public uiMidiTimedItem {
  
    public:
    
        uiMidiStop(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStop()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(1)) {
                fMidiOut->stopSync(0);
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
};

class uiMidiClock : public uiMidiTimedItem {

    private:
        
        bool fState;
  
    public:
    
        uiMidiClock(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input), fState(false)
        {}
        virtual ~uiMidiClock()
        {}
    
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->clock(0);
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                fState = !fState;
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fState));
            }
        }

};

//----------------------
// Standard MIDI events
//----------------------

//---------------------------------------------
// uiMidiProgChange uses the [min...max] range
//---------------------------------------------

class uiMidiProgChange : public uiMidiTimedItem {
    
    public:
    
        FAUSTFLOAT fMin, fMax;
    
        uiMidiProgChange(midi* midi_out, GUI* ui, FAUSTFLOAT* zone,
                         FAUSTFLOAT min, FAUSTFLOAT max,
                         bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fMin(min), fMax(max)
        {}
        virtual ~uiMidiProgChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (inRange(fMin, fMax, v)) {
                fMidiOut->progChange(rangeChan(), v);
            }
        }
    
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl && inRange(fMin, fMax, v)) {
                uiItem::modifyZone(v);
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl && inRange(fMin, fMax, v)) {
                uiMidiTimedItem::modifyZone(date, v);
            }
        }
        
};

class uiMidiChanPress : public uiMidiTimedItem {
    
    private:
        
        int fPress;
  
    public:
    
        uiMidiChanPress(midi* midi_out, int press, GUI* ui, FAUSTFLOAT* zone,
                        bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fPress(press)
        {}
        virtual ~uiMidiChanPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->chanPress(rangeChan(), fPress);
            }
        }
        
};

//------------------------------------------------------
// uiMidiCtrlChange does scale (kLin/kLog/kExp) mapping
//------------------------------------------------------

class uiMidiCtrlChange : public uiMidiTimedItem, public uiConverter {
    
    private:
    
        int fCtrl;
 
    public:

        uiMidiCtrlChange(midi* midi_out, int ctrl, GUI* ui,
                     FAUSTFLOAT* zone,
                     FAUSTFLOAT min, FAUSTFLOAT max,
                     bool input = true,
                     MetaDataUI::Scale scale = MetaDataUI::kLin,
                     int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fCtrl(ctrl)
        {}
        virtual ~uiMidiCtrlChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->ctrlChange(rangeChan(), fCtrl, fConverter->faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
};

class uiMidiPitchWheel : public uiMidiTimedItem {

    private:
    
        LinearValueConverter2 fConverter;
    
    public:
    
        uiMidiPitchWheel(midi* midi_out, GUI* ui, FAUSTFLOAT* zone,
                         FAUSTFLOAT min, FAUSTFLOAT max,
                         bool input = true, int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan)
        {
            if (min <= 0 && max >= 0) {
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min), 0., double(max));
            } else {
                // Degenerated case...
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min),double(min + (max - min)/FAUSTFLOAT(2)), double(max));
            }
        }
    
        virtual ~uiMidiPitchWheel()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->pitchWheel(rangeChan(), fConverter.faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void setRange(int val)
        {
            double semi = (val / 128) + ((val % 128) / 100.);
            fConverter.setMappingValues(0., 8191., 16383., -semi, 0., semi);
        }
 
};

//--------------------------------------------------------------
// uiMidiKeyOn does scale (kLin/kLog/kExp) mapping for velocity
//--------------------------------------------------------------

class uiMidiKeyOn : public uiMidiTimedItem, public uiConverter {

    private:
        
        int fKeyOn;
  
    public:
    
        uiMidiKeyOn(midi* midi_out, int key, GUI* ui,
                    FAUSTFLOAT* zone,
                    FAUSTFLOAT min, FAUSTFLOAT max,
                    bool input = true,
                    MetaDataUI::Scale scale = MetaDataUI::kLin,
                    int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKeyOn(key)
        {}
        virtual ~uiMidiKeyOn()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyOn(rangeChan(), fKeyOn, fConverter->faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

//---------------------------------------------------------------
// uiMidiKeyOff does scale (kLin/kLog/kExp) mapping for velocity
//---------------------------------------------------------------

class uiMidiKeyOff : public uiMidiTimedItem, public uiConverter {

    private:
        
        int fKeyOff;
  
    public:
    
        uiMidiKeyOff(midi* midi_out, int key, GUI* ui,
                     FAUSTFLOAT* zone,
                     FAUSTFLOAT min, FAUSTFLOAT max,
                     bool input = true,
                     MetaDataUI::Scale scale = MetaDataUI::kLin,
                     int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKeyOff(key)
        {}
        virtual ~uiMidiKeyOff()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyOff(rangeChan(), fKeyOff, fConverter->faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

//-----------------------------------------------------------------
// uiMidiKeyPress does scale (kLin/kLog/kExp) mapping for velocity
//-----------------------------------------------------------------

class uiMidiKeyPress : public uiMidiTimedItem, public uiConverter {

    private:
    
        int fKey;
  
    public:
    
        uiMidiKeyPress(midi* midi_out, int key, GUI* ui,
                       FAUSTFLOAT* zone,
                       FAUSTFLOAT min, FAUSTFLOAT max,
                       bool input = true,
                       MetaDataUI::Scale scale = MetaDataUI::kLin,
                       int chan = -1)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKey(key)
        {}
        virtual ~uiMidiKeyPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->keyPress(rangeChan(), fKey, fConverter->faust2ui(v));
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

/******************************************************************************************
 * MidiUI : Faust User Interface
 * This class decodes MIDI metadata and maps incoming MIDI messages to them.
 * Currently ctrl, keyon/keyoff, keypress, pgm, chanpress, pitchwheel/pitchbend
 * start/stop/clock meta data are handled.
 *
 * Maps associating MIDI event ID (like each ctrl number) with all MIDI aware UI items
 * are defined and progressively filled when decoding MIDI related metadata.
 * MIDI aware UI items are used in both directions:
 *  - modifying their internal state when receving MIDI input events
 *  - sending their internal state as MIDI output events
 *******************************************************************************************/

class MidiUI : public GUI, public midi, public MetaDataUI {

    // Add uiItem subclasses objects are deallocated by the inherited GUI class
    typedef std::map <int, std::vector<uiMidiCtrlChange*> > TCtrlChangeTable;
    typedef std::vector<uiMidiProgChange*>                  TProgChangeTable;
    typedef std::map <int, std::vector<uiMidiChanPress*> >  TChanPressTable;
    typedef std::map <int, std::vector<uiMidiKeyOn*> >      TKeyOnTable;
    typedef std::map <int, std::vector<uiMidiKeyOff*> >     TKeyOffTable;
    typedef std::map <int, std::vector<uiMidiKeyPress*> >   TKeyPressTable;
    typedef std::vector<uiMidiPitchWheel*>                  TPitchWheelTable;
    
    protected:
    
        TCtrlChangeTable fCtrlChangeTable;
        TProgChangeTable fProgChangeTable;
        TChanPressTable  fChanPressTable;
        TKeyOnTable      fKeyOnTable;
        TKeyOffTable     fKeyOffTable;
        TKeyOnTable      fKeyTable;
        TKeyPressTable   fKeyPressTable;
        TPitchWheelTable fPitchWheelTable;
        
        std::vector<uiMidiStart*> fStartTable;
        std::vector<uiMidiStop*>  fStopTable;
        std::vector<uiMidiClock*> fClockTable;
        
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        
        midi_handler* fMidiHandler;
        bool fDelete;
        bool fTimeStamp;
    
        void addGenericZone(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true)
        {
            if (fMetaAux.size() > 0) {
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    unsigned num;
                    unsigned chan;
                    if (fMetaAux[i].first == "midi") {
                        if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u %u", &num, &chan) == 2) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u", &num) == 1) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u %u", &num, &chan) == 2) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u", &num) == 1) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u %u", &num, &chan) == 2) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u", &num) == 1) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u %u", &num, &chan) == 2) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u", &num) == 1) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u %u", &num, &chan) == 2) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u", &num) == 1) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "pgm %u", &chan) == 1) {
                            fProgChangeTable.push_back(new uiMidiProgChange(fMidiHandler, this, zone, min, max, input, chan));
                        } else if (strcmp(fMetaAux[i].second.c_str(), "pgm") == 0) {
                            fProgChangeTable.push_back(new uiMidiProgChange(fMidiHandler, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "chanpress %u %u", &num, &chan) == 2) {
                            fChanPressTable[num].push_back(new uiMidiChanPress(fMidiHandler, num, this, zone, input, chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "chanpress %u", &num) == 1) {
                            fChanPressTable[num].push_back(new uiMidiChanPress(fMidiHandler, num, this, zone, input));
                        } else if ((gsscanf(fMetaAux[i].second.c_str(), "pitchwheel %u", &chan) == 1) || (gsscanf(fMetaAux[i].second.c_str(), "pitchbend %u", &chan) == 1)) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input, chan));
                        } else if ((fMetaAux[i].second == "pitchwheel") || (fMetaAux[i].second == "pitchbend")) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input));
                        // MIDI sync
                        } else if (fMetaAux[i].second == "start") {
                            fStartTable.push_back(new uiMidiStart(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "stop") {
                            fStopTable.push_back(new uiMidiStop(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "clock") {
                            fClockTable.push_back(new uiMidiClock(fMidiHandler, this, zone, input));
                        // Explicit metadata to activate 'timestamp' mode
                        } else if (fMetaAux[i].second == "timestamp") {
                            fTimeStamp = true;
                        }
                    }
                }
            }
            fMetaAux.clear();
        }
    
        template <typename TABLE>
        void updateTable1(TABLE& table, double date, int channel, int val1)
        {
            for (size_t i = 0; i < table.size(); i++) {
                int channel_aux = table[i]->fChan;
                if (channel_aux == -1 || channel == channel_aux) {
                    if (fTimeStamp) {
                        table[i]->modifyZone(date, FAUSTFLOAT(val1));
                    } else {
                        table[i]->modifyZone(FAUSTFLOAT(val1));
                    }
                }
            }
        }
        
        template <typename TABLE>
        void updateTable2(TABLE& table, double date, int channel, int val1, int val2)
        {
            if (table.find(val1) != table.end()) {
                for (size_t i = 0; i < table[val1].size(); i++) {
                    int channel_aux = table[val1][i]->fChan;
                    if (channel_aux == -1 || channel == channel_aux) {
                        if (fTimeStamp) {
                            table[val1][i]->modifyZone(date, FAUSTFLOAT(val2));
                        } else {
                            table[val1][i]->modifyZone(FAUSTFLOAT(val2));
                        }
                    }
                }
            }
        }
    
    public:
    
        MidiUI(midi_handler* midi_handler, bool delete_handler = false)
        {
            fMidiHandler = midi_handler;
            fMidiHandler->addMidiIn(this);
            fDelete = delete_handler;
            fTimeStamp = false;
        }
 
        virtual ~MidiUI() 
        { 
            fMidiHandler->removeMidiIn(this);
            if (fDelete) delete fMidiHandler;
        }
    
        bool run() { return fMidiHandler->startMidi(); }
        void stop() { fMidiHandler->stopMidi(); }
        
        void addMidiIn(midi* midi_dsp) { fMidiHandler->addMidiIn(midi_dsp); }
        void removeMidiIn(midi* midi_dsp) { fMidiHandler->removeMidiIn(midi_dsp); }
      
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
        {
            addGenericZone(zone, min, max, false);
        }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addGenericZone(zone, min, max, false);
        }

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {
            MetaDataUI::declare(zone, key, val);
            fMetaAux.push_back(std::make_pair(key, val));
        }
        
        // -- MIDI API
    
        void key(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyTable, date, channel, note, velocity);
        }
    
        MapUI* keyOn(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyOnTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOn
            key(date, channel, note, velocity);
            return nullptr;
        }
        
        void keyOff(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOffTable>(fKeyOffTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOff with a 0 velocity
            key(date, channel, note, 0);
        }
        
        void ctrlChange(double date, int channel, int ctrl, int value)
        {
            updateTable2<TCtrlChangeTable>(fCtrlChangeTable, date, channel, ctrl, value);
        }
    
        void rpn(double date, int channel, int ctrl, int value)
        {
            if (ctrl == midi::PITCH_BEND_RANGE) {
                for (size_t i = 0; i < fPitchWheelTable.size(); i++) {
                    int channel_aux = fPitchWheelTable[i]->fChan;
                    if (channel_aux == -1 || channel == channel_aux) {
                        fPitchWheelTable[i]->setRange(value);
                    }
                }
            }
        }
    
        void progChange(double date, int channel, int pgm)
        {
            updateTable1<TProgChangeTable>(fProgChangeTable, date, channel, pgm);
        }
        
        void pitchWheel(double date, int channel, int wheel) 
        {
            updateTable1<TPitchWheelTable>(fPitchWheelTable, date, channel, wheel);
        }
        
        void keyPress(double date, int channel, int pitch, int press) 
        {
            updateTable2<TKeyPressTable>(fKeyPressTable, date, channel, pitch, press);
        }
        
        void chanPress(double date, int channel, int press)
        {
            updateTable2<TChanPressTable>(fChanPressTable, date, channel, press, FAUSTFLOAT(1));
        }
        
        void ctrlChange14bits(double date, int channel, int ctrl, int value) {}
        
        // MIDI sync
        
        void startSync(double date)
        {
            for (size_t i = 0; i < fStartTable.size(); i++) {
                fStartTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
        
        void stopSync(double date)
        {
            for (size_t i = 0; i < fStopTable.size(); i++) {
                fStopTable[i]->modifyZone(date, FAUSTFLOAT(0));
            }
        }
        
        void clock(double date)
        {
            for (size_t i = 0; i < fClockTable.size(); i++) {
                fClockTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
};

#endif // FAUST_MIDIUI_H
/**************************  END  MidiUI.h **************************/
/************************** BEGIN mspUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
//
//  mspUI.h for static Max/MSP externals and faustgen~
//
//  Created by Martin Di Rollo on 18/04/12.
//  Copyright (c) 2012-2019 Grame. All rights reserved.
//

#ifndef _mspUI_h
#define _mspUI_h

#include <math.h>
#include <string>
#include <map>


#define STR_SIZE    512
#define MULTI_SIZE  256

#ifdef WIN32
#include <stdio.h>
#define snprintf _snprintf
#ifndef NAN
    static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
    #define NAN (*(const float *) __nan)
#endif
#endif

using namespace std;

struct Max_Meta1 : Meta
{
    int fCount;
    
    Max_Meta1():fCount(0)
    {}
    
    void declare(const char* key, const char* value)
    {
        if ((strcmp("name", key) == 0) || (strcmp("author", key) == 0)) {
            fCount++;
        }
    }
};

struct Max_Meta2 : Meta
{
    void declare(const char* key, const char* value)
    {
        if ((strcmp("name", key) == 0) || (strcmp("author", key) == 0)) {
            post("%s : %s", key, value);
        }
    }
};

struct Max_Meta3 : Meta
{
    string fName;
    
    bool endWith(const string& str, const string& suffix)
    {
        size_t i = str.rfind(suffix);
        return (i != string::npos) && (i == (str.length() - suffix.length()));
    }
    
    void declare(const char* key, const char* value)
    {
        if ((strcmp("filename", key) == 0)) {
            string val = value;
            if (endWith(value, ".dsp")) {
                fName = "com.grame." + val.substr(0, val.size() - 4) + "~";
            } else {
                fName = "com.grame." + val + "~";
            }
        }
    }
};

class mspUIObject {
    
    protected:
        
        string fLabel;
        FAUSTFLOAT* fZone;
        
        FAUSTFLOAT range(FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT val) {return (val < min) ? min : (val > max) ? max : val;}
        
    public:
        
        mspUIObject(const string& label, FAUSTFLOAT* zone):fLabel(label),fZone(zone) {}
        virtual ~mspUIObject() {}
        
        virtual void setValue(FAUSTFLOAT f) { *fZone = range(0.0, 1.0, f); }
        virtual FAUSTFLOAT getValue() { return *fZone; }
    
        virtual FAUSTFLOAT getInitValue() { return FAUSTFLOAT(0); }
        virtual FAUSTFLOAT getMinValue() { return FAUSTFLOAT(0); }
        virtual FAUSTFLOAT getMaxValue() { return FAUSTFLOAT(0); }
    
        virtual void toString(char* buffer) {}
        virtual string getName() { return fLabel; }
};

class mspCheckButton : public mspUIObject {
    
    public:
        
        mspCheckButton(const string& label, FAUSTFLOAT* zone):mspUIObject(label,zone) {}
        virtual ~mspCheckButton() {}
        
        void toString(char* buffer)
        {
            snprintf(buffer, STR_SIZE, "CheckButton(float): %s", fLabel.c_str());
        }
};

class mspButton : public mspUIObject {
    
    public:
        
        mspButton(const string& label, FAUSTFLOAT* zone):mspUIObject(label,zone) {}
        virtual ~mspButton() {}
        
        void toString(char* buffer)
        {
            snprintf(buffer, STR_SIZE, "Button(float): %s", fLabel.c_str());
        }
};

class mspSlider : public mspUIObject {
    
    private:
        
        FAUSTFLOAT fInit;
        FAUSTFLOAT fMin;
        FAUSTFLOAT fMax;
        FAUSTFLOAT fStep;
        
    public:
        
        mspSlider(const string& label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        :mspUIObject(label,zone),fInit(init),fMin(min),fMax(max),fStep(step) {}
        virtual ~mspSlider() {}
        
        void toString(char* buffer)
        {
            stringstream str;
            str << "Slider(float): " << fLabel << " [init=" << fInit << ":min=" << fMin << ":max=" << fMax << ":step=" << fStep << ":cur=" << *fZone << "]";
            string res = str.str();
            snprintf(buffer, STR_SIZE, "%s", res.c_str());
        }
        
        void setValue(FAUSTFLOAT f) { *fZone = range(fMin, fMax, f); }
    
        virtual FAUSTFLOAT getInitValue() { return fInit; }
        virtual FAUSTFLOAT getMinValue() { return fMin; }
        virtual FAUSTFLOAT getMaxValue() { return fMax; }
    
};

class mspBargraph : public mspUIObject {
    
    private:
        
        FAUSTFLOAT fMin;
        FAUSTFLOAT fMax;
        FAUSTFLOAT fCurrent;
        
    public:
        
        mspBargraph(const string& label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        :mspUIObject(label,zone), fMin(min), fMax(max), fCurrent(*zone) {}
        virtual ~mspBargraph() {}
        
        void toString(char* buffer)
        {
            stringstream str;
            str << "Bargraph(float): " << fLabel << " [min=" << fMin << ":max=" << fMax << ":cur=" << *fZone << "]";
            string res = str.str();
            snprintf(buffer, STR_SIZE, "%s", res.c_str());
        }
    
        // special version
        virtual FAUSTFLOAT getValue(bool& new_val)
        {
            if (*fZone != fCurrent) {
                fCurrent = *fZone;
                new_val = true;
            } else {
                new_val = false;
            }
            return fCurrent;
        }
    
        virtual FAUSTFLOAT getMinValue() { return fMin; }
        virtual FAUSTFLOAT getMaxValue() { return fMax; }
    
};

class mspUI : public UI, public PathBuilder
{
    private:
        
        map<string, mspUIObject*> fInputLabelTable;      // Input table using labels
        map<string, mspUIObject*> fInputPathTable;       // Input table using paths
        map<string, mspUIObject*> fOutputLabelTable;     // Table containing bargraph with labels
        map<string, mspUIObject*> fOutputPathTable;      // Table containing bargraph with paths
        
        map<const char*, const char*> fDeclareTable;
        
        FAUSTFLOAT* fMultiTable[MULTI_SIZE];
        int fMultiIndex;
        int fMultiControl;
        
        string createLabel(const char* label)
        {
            map<const char*, const char*>::reverse_iterator it;
            if (fDeclareTable.size() > 0) {
                unsigned int i = 0;
                string res = string(label);
                char sep = '[';
                for (it = fDeclareTable.rbegin(); it != fDeclareTable.rend(); it++, i++) {
                    res = res + sep + (*it).first + ":" + (*it).second;
                    sep = ',';
                }
                res += ']';
                fDeclareTable.clear();
                return res;
            } else {
                return string(label);
            }
        }
    
        void addSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            mspUIObject* obj = new mspSlider(createLabel(label), zone, init, min, max, step);
            fInputLabelTable[string(label)] = obj;
            fInputPathTable[buildPath(label)] = obj;
            fDeclareTable.clear();
        }
    
        void addBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            mspUIObject* obj = new mspBargraph(createLabel(label), zone, min, max);
            fOutputLabelTable[string(label)] = obj;
            fOutputPathTable[buildPath(label)] = obj;
            fDeclareTable.clear();
        }
    
    public:
        
        typedef map<string, mspUIObject*>::iterator iterator;
        
        mspUI()
        {
            for (int i = 0; i < MULTI_SIZE; i++) {
                fMultiTable[i] = 0;
            }
            fMultiIndex = fMultiControl = 0;
        }
    
        virtual ~mspUI()
        {
            clear();
        }
        
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            mspUIObject* obj = new mspButton(createLabel(label), zone);
            fInputLabelTable[string(label)] = obj;
            fInputPathTable[buildPath(label)] = obj;
        }
        
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            mspUIObject* obj = new mspCheckButton(createLabel(label), zone);
            fInputLabelTable[string(label)] = obj;
            fInputPathTable[buildPath(label)] = obj;
        }
        
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addSlider(label, zone, init, min, max, step);
        }
        
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addSlider(label, zone, init, min, max, step);
        }
        
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addSlider(label, zone, init, min, max, step);
        }
    
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addBargraph(label, zone, min, max);
        }
    
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addBargraph(label, zone, min, max);
        }
        
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        void openTabBox(const char* label) { pushLabel(label); fDeclareTable.clear(); }
        void openHorizontalBox(const char* label) { pushLabel(label); fDeclareTable.clear(); }
        void openVerticalBox(const char* label) { pushLabel(label); fDeclareTable.clear(); }
        void closeBox() { popLabel(); fDeclareTable.clear(); }
        
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {
            if (strcmp(key, "multi") == 0) {
                int index = atoi(val);
                if (index >= 0 && index < MULTI_SIZE) {
                    fMultiTable[index] = zone;
                    fMultiControl++;
                } else {
                    post("Invalid multi index = %d", index);
                }
            }
            
            fDeclareTable[key] = val;
        }
        
        void setMultiValues(FAUSTFLOAT* multi, int buffer_size)
        {
            for (int read = 0; read < buffer_size; read++) {
                int write = (fMultiIndex + read) & (MULTI_SIZE - 1);
                if (fMultiTable[write]) {
                    *fMultiTable[write] = multi[read];
                }
            }
            fMultiIndex += buffer_size;
        }
        
        bool isMulti() { return fMultiControl > 0; }
        
        bool isValue(const string& name)
        {
            return (fInputLabelTable.count(name) || fInputPathTable.count(name));
        }
    
        bool isOutputValue(const string& name)
        {
            return fOutputPathTable.count(name);
        }
    
        bool isInputValue(const string& name)
        {
            return fInputPathTable.count(name);
        }
    
        bool setValue(const string& name, FAUSTFLOAT val)
        {
            if (fInputLabelTable.count(name)) {
                fInputLabelTable[name]->setValue(val);
                return true;
            } else if (fInputPathTable.count(name)) {
                fInputPathTable[name]->setValue(val);
                return true;
            } else {
                return false;
            }
        }
    
        FAUSTFLOAT getOutputValue(const string& name, bool& new_val)
        {
            return static_cast<mspBargraph*>(fOutputPathTable[name])->getValue(new_val);
        }
        
        iterator begin1() { return fInputLabelTable.begin(); }
        iterator end1() { return fInputLabelTable.end(); }
        
        iterator begin2() { return fInputPathTable.begin(); }
        iterator end2() { return fInputPathTable.end(); }
    
        iterator begin3() { return fOutputLabelTable.begin(); }
        iterator end3() { return fOutputLabelTable.end(); }
    
        iterator begin4() { return fOutputPathTable.begin(); }
        iterator end4() { return fOutputPathTable.end(); }
    
        int inputItemsCount() { return fInputLabelTable.size(); }
        int outputItemsCount() { return fOutputLabelTable.size(); }
    
        void clear()
        {
            for (auto& it : fInputLabelTable) {
                delete it.second;
            }
            fInputLabelTable.clear();
            fInputPathTable.clear();
            
            for (auto& it : fOutputLabelTable) {
                delete it.second;
            }
            fOutputLabelTable.clear();
            fOutputPathTable.clear();
        }
        
        void displayControls()
        {
            post("------- Range and path ----------");
            for (auto& it : fInputPathTable) {
                char param[STR_SIZE];
                it.second->toString(param);
                post(param);
                string path = "Complete path: " + it.first;
                post(path.c_str());
            }
            post("---------------------------------");
        }
    
        static bool checkDigit(const string& name)
        {
            for (int i = name.size() - 1; i >= 0; i--) {
                if (isdigit(name[i])) { return true; }
            }
            return false;
        }
        
        static int countDigit(const string& name)
        {
            int count = 0;
            for (int i = name.size() - 1; i >= 0; i--) {
                if (isdigit(name[i])) { count++; }
            }
            return count;
        }

};

#endif
/**************************  END  mspUI.h **************************/
/************************** BEGIN poly-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __poly_dsp__
#define __poly_dsp__

#include <stdio.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <float.h>
#include <assert.h>

/************************** BEGIN proxy-dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __proxy_dsp__
#define __proxy_dsp__

#include <vector>
#include <map>

/************************** BEGIN JSONUIDecoder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JSONUIDecoder__
#define __JSONUIDecoder__

#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <sstream>
#include <functional>

/************************** BEGIN CGlue.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef CGLUE_H
#define CGLUE_H

/************************** BEGIN CInterface.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2018 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef CINTERFACE_H
#define CINTERFACE_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
struct Soundfile;

/*******************************************************************************
 * UI, Meta and MemoryManager structures for C code.
 ******************************************************************************/

// -- widget's layouts

typedef void (* openTabBoxFun) (void* ui_interface, const char* label);
typedef void (* openHorizontalBoxFun) (void* ui_interface, const char* label);
typedef void (* openVerticalBoxFun) (void* ui_interface, const char* label);
typedef void (* closeBoxFun) (void* ui_interface);

// -- active widgets

typedef void (* addButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addCheckButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addVerticalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addHorizontalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addNumEntryFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

// -- passive widgets

typedef void (* addHorizontalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);
typedef void (* addVerticalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

// -- soundfiles
    
typedef void (* addSoundfileFun) (void* ui_interface, const char* label, const char* url, struct Soundfile** sf_zone);

typedef void (* declareFun) (void* ui_interface, FAUSTFLOAT* zone, const char* key, const char* value);

typedef struct {

    void* uiInterface;

    openTabBoxFun openTabBox;
    openHorizontalBoxFun openHorizontalBox;
    openVerticalBoxFun openVerticalBox;
    closeBoxFun closeBox;
    addButtonFun addButton;
    addCheckButtonFun addCheckButton;
    addVerticalSliderFun addVerticalSlider;
    addHorizontalSliderFun addHorizontalSlider;
    addNumEntryFun addNumEntry;
    addHorizontalBargraphFun addHorizontalBargraph;
    addVerticalBargraphFun addVerticalBargraph;
    addSoundfileFun addSoundfile;
    declareFun declare;

} UIGlue;

typedef void (* metaDeclareFun) (void* ui_interface, const char* key, const char* value);

typedef struct {

    void* metaInterface;
    
    metaDeclareFun declare;

} MetaGlue;

/***************************************
 *  Interface for the DSP object
 ***************************************/

typedef char dsp_imp;
    
typedef dsp_imp* (* newDspFun) ();
typedef void (* destroyDspFun) (dsp_imp* dsp);
typedef int (* getNumInputsFun) (dsp_imp* dsp);
typedef int (* getNumOutputsFun) (dsp_imp* dsp);
typedef void (* buildUserInterfaceFun) (dsp_imp* dsp, UIGlue* ui);
typedef int (* getSampleRateFun) (dsp_imp* dsp);
typedef void (* initFun) (dsp_imp* dsp, int sample_rate);
typedef void (* classInitFun) (int sample_rate);
typedef void (* instanceInitFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceConstantsFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceResetUserInterfaceFun) (dsp_imp* dsp);
typedef void (* instanceClearFun) (dsp_imp* dsp);
typedef void (* computeFun) (dsp_imp* dsp, int len, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs);
typedef void (* metadataFun) (MetaGlue* meta);
    
/***************************************
 * DSP memory manager functions
 ***************************************/

typedef void* (* allocateFun) (void* manager_interface, size_t size);
typedef void (* destroyFun) (void* manager_interface, void* ptr);

typedef struct {
    
    void* managerInterface;
    
    allocateFun allocate;
    destroyFun destroy;
    
} MemoryManagerGlue;

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CInterface.h **************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * UI glue code
 ******************************************************************************/
 
class UIFloat
{

    public:

        UIFloat() {}

        virtual ~UIFloat() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, float* zone) = 0;
        virtual void addCheckButton(const char* label, float* zone) = 0;
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max) = 0;
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(float* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueFloat(void* cpp_interface)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueFloat(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueFloat(void* cpp_interface, float* zone, const char* key, const char* value)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

class UIDouble
{

    public:

        UIDouble() {}

        virtual ~UIDouble() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, double* zone) = 0;
        virtual void addCheckButton(const char* label, double* zone) = 0;
        virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max) = 0;
        virtual void addVerticalBargraph(const char* label, double* zone, double min, double max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(double* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueDouble(void* cpp_interface)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueDouble(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueDouble(void* cpp_interface, double* zone, const char* key, const char* value)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

static void buildUIGlue(UIGlue* glue, UI* ui_interface, bool is_double)
{
    glue->uiInterface = ui_interface;
    
    if (is_double) {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueDouble);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueDouble);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueDouble);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueDouble);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueDouble);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueDouble);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueDouble);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueDouble);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueDouble);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueDouble);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueDouble);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueDouble);
        glue->declare = reinterpret_cast<declareFun>(declareGlueDouble);
    } else {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueFloat);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueFloat);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueFloat);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueFloat);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueFloat);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueFloat);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueFloat);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueFloat);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueFloat);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueFloat);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueFloat);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueFloat);
        glue->declare = reinterpret_cast<declareFun>(declareGlueFloat);
    }
}
    
class UITemplate
{
    
    private:
        
        void* fCPPInterface;
        
    public:
        
        UITemplate(void* cpp_interface):fCPPInterface(cpp_interface)
        {}
        
        virtual ~UITemplate() {}
        
        // -- widget's layouts
        
        virtual void openTabBox(const char* label)
        {
            openTabBoxGlueFloat(fCPPInterface, label);
        }
        virtual void openHorizontalBox(const char* label)
        {
            openHorizontalBoxGlueFloat(fCPPInterface, label);
        }
        virtual void openVerticalBox(const char* label)
        {
            openVerticalBoxGlueFloat(fCPPInterface, label);
        }
        virtual void closeBox()
        {
            closeBoxGlueFloat(fCPPInterface);
        }
        
        // float version
        
        // -- active widgets
        
        virtual void addButton(const char* label, float* zone)
        {
            addButtonGlueFloat(fCPPInterface, label, zone);
        }
        virtual void addCheckButton(const char* label, float* zone)
        {
            addCheckButtonGlueFloat(fCPPInterface, label, zone);
        }
        
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
        {
            addVerticalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
        {
            addHorizontalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
        {
            addNumEntryGlueFloat(fCPPInterface, label, zone, init, min, max, step);
        }
        
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max)
        {
            addHorizontalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
        }
        
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max)
        {
            addVerticalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
        }
    
        // -- metadata declarations
        
        virtual void declare(float* zone, const char* key, const char* val)
        {
            declareGlueFloat(fCPPInterface, zone, key, val);
        }
        
        // double version
        
        virtual void addButton(const char* label, double* zone)
        {
            addButtonGlueDouble(fCPPInterface, label, zone);
        }
        virtual void addCheckButton(const char* label, double* zone)
        {
            addCheckButtonGlueDouble(fCPPInterface, label, zone);
        }
        
        virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step)
        {
            addVerticalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step)
        {
            addHorizontalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
        
        virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step)
        {
            addNumEntryGlueDouble(fCPPInterface, label, zone, init, min, max, step);
        }
    
        // -- soundfiles
        
        virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
        {
            addSoundfileGlueFloat(fCPPInterface, label, url, sf_zone);
        }
    
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max)
        {
            addHorizontalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
        }
        
        virtual void addVerticalBargraph(const char* label, double* zone, double min, double max)
        {
            addVerticalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
        }
    
        // -- metadata declarations
        
        virtual void declare(double* zone, const char* key, const char* val)
        {
            declareGlueDouble(fCPPInterface, zone, key, val);
        }

};

/*******************************************************************************
 * Meta glue code
 ******************************************************************************/

static void declareMetaGlue(void* cpp_interface, const char* key, const char* value)
{
    Meta* meta_interface = static_cast<Meta*>(cpp_interface);
    meta_interface->declare(key, value);
}

static void buildMetaGlue(MetaGlue* glue, Meta* meta)
{
    glue->metaInterface = meta;
    glue->declare = declareMetaGlue;
}
    
/*******************************************************************************
 * Memory manager glue code
 ******************************************************************************/

static void* allocateMemoryManagerGlue(void* cpp_interface, size_t size)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    return manager_interface->allocate(size);
}
    
static void destroyMemoryManagerGlue(void* cpp_interface, void* ptr)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    manager_interface->destroy(ptr);
}

static void buildManagerGlue(MemoryManagerGlue* glue, dsp_memory_manager* manager)
{
    glue->managerInterface = manager;
    glue->allocate = allocateMemoryManagerGlue;
    glue->destroy = destroyMemoryManagerGlue;
}

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CGlue.h **************************/

#ifdef _WIN32
#include <windows.h>
#define snprintf _snprintf
#endif

//-------------------------------------------------------------------
//  Decode a dsp JSON description and implement 'buildUserInterface'
//-------------------------------------------------------------------

#define REAL_UI(ui_interface) reinterpret_cast<UIReal<REAL>*>(ui_interface)
#define REAL_ADR(offset)      reinterpret_cast<REAL*>(&memory_block[offset])
#define REAL_EXT_ADR(offset)  reinterpret_cast<FAUSTFLOAT*>(&memory_block[offset])
#define SOUNDFILE_ADR(offset) reinterpret_cast<Soundfile**>(&memory_block[offset])

typedef std::function<void(double)> ReflectFunction;
typedef std::function<double()> ModifyFunction;

struct ExtZoneParam {

    virtual void reflectZone() = 0;
    virtual void modifyZone() = 0;
    
    virtual void setReflectZoneFun(ReflectFunction reflect) = 0;
    virtual void setModifyZoneFun(ModifyFunction modify) = 0;
    
    virtual ~ExtZoneParam()
    {}
    
};

template <typename REAL>
struct JSONUIDecoderReal {
    
    struct ZoneParam : public ExtZoneParam {
        
        REAL fZone;
        int fIndex;
        ReflectFunction fReflect;
        ModifyFunction fModify;
        
    #if defined(TARGET_OS_IPHONE) || defined(WIN32)
        ZoneParam(int index, ReflectFunction reflect = nullptr, ModifyFunction modify = nullptr)
        :fIndex(index), fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { if (fReflect) fReflect(fZone); }
        void modifyZone() { if (fModify) fZone = fModify(); }
    #else
        ZoneParam(int index, ReflectFunction reflect = [](REAL value) {}, ModifyFunction modify = []() { return REAL(-1); })
        :fIndex(index), fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { fReflect(fZone); }
        void modifyZone() { fZone = fModify(); }
    #endif
        
        void setReflectZoneFun(ReflectFunction reflect) { fReflect = reflect; }
        void setModifyZoneFun(ModifyFunction modify) { fModify = modify; }
        
    };

    typedef std::vector<ExtZoneParam*> controlMap;
  
    std::string fName;
    std::string fFileName;
    std::string fJSON;
    std::string fVersion;
    std::string fCompileOptions;
    
    std::map<std::string, std::string> fMetadata;
    std::vector<itemInfo> fUiItems;
    
    std::vector<std::string> fLibraryList;
    std::vector<std::string> fIncludePathnames;
    
    Soundfile** fSoundfiles;
    
    int fNumInputs, fNumOutputs, fSRIndex;
    int fSoundfileItems;
    int fDSPSize;
    
    controlMap fPathInputTable;     // [path, ZoneParam]
    controlMap fPathOutputTable;    // [path, ZoneParam]

    bool isInput(const std::string& type)
    {
        return (type == "vslider" || type == "hslider" || type == "nentry" || type == "button" || type == "checkbox");
    }
    bool isOutput(const std::string& type) { return (type == "hbargraph" || type == "vbargraph"); }
    bool isSoundfile(const std::string& type) { return (type == "soundfile"); }
    
    std::string getString(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? map[key].first : "";
    }
    
    int getInt(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? int(map[key].second) : -1;
    }
    
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        fPathInputTable[index]->setReflectZoneFun(fun);
    }
    
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        fPathOutputTable[index]->setModifyZoneFun(fun);
    }

    JSONUIDecoderReal(const std::string& json)
    {
        fJSON = json;
        const char* p = fJSON.c_str();
        std::map<std::string, std::pair<std::string, double> > meta_data1;
        std::map<std::string, std::vector<std::string> > meta_data2;
        parseJson(p, meta_data1, fMetadata, meta_data2, fUiItems);
        
        // meta_data1 contains <name : val>, <inputs : val>, <ouputs : val> pairs etc...
        fName = getString(meta_data1, "name");
        fFileName = getString(meta_data1, "filename");
        fVersion = getString(meta_data1, "version");
        fCompileOptions = getString(meta_data1, "compile_options");
        
        if (meta_data2.find("library_list") != meta_data2.end()) {
            fLibraryList = meta_data2["library_list"];
        }
        if (meta_data2.find("include_pathnames") != meta_data2.end()) {
            fIncludePathnames = meta_data2["include_pathnames"];
        }
        
        fDSPSize = getInt(meta_data1, "size");
        fNumInputs = getInt(meta_data1, "inputs");
        fNumOutputs = getInt(meta_data1, "outputs");
        fSRIndex = getInt(meta_data1, "sr_index");
       
        fSoundfileItems = 0;
        for (auto& it : fUiItems) {
            std::string type = it.type;
            if (isSoundfile(type)) {
                fSoundfileItems++;
            }
        }
        
        fSoundfiles = new Soundfile*[fSoundfileItems];
        
        // Prepare the fPathTable and init zone
        for (auto& it : fUiItems) {
            std::string type = it.type;
            // Meta data declaration for input items
            if (isInput(type)) {
                ZoneParam* param = new ZoneParam(it.index);
                fPathInputTable.push_back(param);
                param->fZone = it.init;
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                ZoneParam* param = new ZoneParam(it.index);
                fPathOutputTable.push_back(param);
                param->fZone = REAL(0);
            }
        }
    }
    
    virtual ~JSONUIDecoderReal()
    {
        delete [] fSoundfiles;
        for (auto& it : fPathInputTable) {
            delete it;
        }
        for (auto& it : fPathOutputTable) {
            delete it;
        }
    }
    
    void metadata(Meta* m)
    {
        for (auto& it : fMetadata) {
            m->declare(it.first.c_str(), it.second.c_str());
        }
    }
    
    void metadata(MetaGlue* m)
    {
        for (auto& it : fMetadata) {
            m->declare(m->metaInterface, it.first.c_str(), it.second.c_str());
        }
    }
    
    void resetUserInterface()
    {
        int item = 0;
        for (auto& it : fUiItems) {
            if (isInput(it.type)) {
                static_cast<ZoneParam*>(fPathInputTable[item++])->fZone = it.init;
            }
        }
    }
    
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        for (auto& it : fUiItems) {
            int offset = it.index;
            if (isInput(it.type)) {
                *REAL_ADR(offset) = it.init;
            } else if (isSoundfile(it.type)) {
                if (*SOUNDFILE_ADR(offset) == nullptr) {
                    *SOUNDFILE_ADR(offset) = defaultsound;
                }
            }
        }
    }
    
    int getSampleRate(char* memory_block)
    {
        return *reinterpret_cast<int*>(&memory_block[fSRIndex]);
    }
   
    void buildUserInterface(UI* ui_interface)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        int countIn = 0;
        int countOut = 0;
        int countSound = 0;
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(&static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(&static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                REAL_UI(ui_interface)->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") { 
                REAL_UI(ui_interface)->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                REAL_UI(ui_interface)->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                REAL_UI(ui_interface)->addVerticalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "hslider") {
                REAL_UI(ui_interface)->addHorizontalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "checkbox") {
                REAL_UI(ui_interface)->addCheckButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "soundfile") {
                REAL_UI(ui_interface)->addSoundfile(it.label.c_str(), it.url.c_str(), &fSoundfiles[countSound]);
            } else if (type == "hbargraph") {
                REAL_UI(ui_interface)->addHorizontalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "vbargraph") {
                REAL_UI(ui_interface)->addVerticalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "nentry") {
                REAL_UI(ui_interface)->addNumEntry(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "button") {
                REAL_UI(ui_interface)->addButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "close") {
                REAL_UI(ui_interface)->closeBox();
            }
            
            if (isInput(type)) {
                countIn++;
            } else if (isOutput(type)) {
                countOut++;
            } else if (isSoundfile(type)) {
                countSound++;
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            int offset = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                REAL_UI(ui_interface)->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") {
                REAL_UI(ui_interface)->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                REAL_UI(ui_interface)->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                REAL_UI(ui_interface)->addVerticalSlider(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "hslider") {
                REAL_UI(ui_interface)->addHorizontalSlider(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "checkbox") {
                REAL_UI(ui_interface)->addCheckButton(it.label.c_str(), REAL_ADR(offset));
            } else if (type == "soundfile") {
                REAL_UI(ui_interface)->addSoundfile(it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(offset));
            } else if (type == "hbargraph") {
                REAL_UI(ui_interface)->addHorizontalBargraph(it.label.c_str(), REAL_ADR(offset), min, max);
            } else if (type == "vbargraph") {
                REAL_UI(ui_interface)->addVerticalBargraph(it.label.c_str(), REAL_ADR(offset), min, max);
            } else if (type == "nentry") {
                REAL_UI(ui_interface)->addNumEntry(it.label.c_str(), REAL_ADR(offset), init, min, max, step);
            } else if (type == "button") {
                REAL_UI(ui_interface)->addButton(it.label.c_str(), REAL_ADR(offset));
            } else if (type == "close") {
                REAL_UI(ui_interface)->closeBox();
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = strdup(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (auto& it : fUiItems) {
            
            std::string type = it.type;
            int offset = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.min);
            REAL max = REAL(it.max);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(offset), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, 0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                ui_interface->openHorizontalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vgroup") {
                ui_interface->openVerticalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "tgroup") {
                ui_interface->openTabBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vslider") {
                ui_interface->addVerticalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "hslider") {
                ui_interface->addHorizontalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "checkbox") {
                ui_interface->addCheckButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset));
            } else if (type == "soundfile") {
                ui_interface->addSoundfile(ui_interface->uiInterface, it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(offset));
            } else if (type == "hbargraph") {
                ui_interface->addHorizontalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), min, max);
            } else if (type == "vbargraph") {
                ui_interface->addVerticalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), min, max);
            } else if (type == "nentry") {
                ui_interface->addNumEntry(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset), init, min, max, step);
            } else if (type == "button") {
                ui_interface->addButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(offset));
            } else if (type == "close") {
                ui_interface->closeBox(ui_interface->uiInterface);
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    bool hasCompileOption(const std::string& option)
    {
        std::istringstream iss(fCompileOptions);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            if (token == option) return true;
        }
        return false;
    }
    
};

// Templated decoder

struct JSONUITemplatedDecoder
{

    virtual ~JSONUITemplatedDecoder()
    {}
    
    virtual void metadata(Meta* m) = 0;
    virtual void metadata(MetaGlue* glue) = 0;
    virtual int getDSPSize() = 0;
    virtual std::string getName() = 0;
    virtual std::string getLibVersion() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;
    virtual int getNumInputs() = 0;
    virtual int getNumOutputs() = 0;
    virtual int getSampleRate(char* memory_block) = 0;
    virtual void setReflectZoneFun(int index, ReflectFunction fun) = 0;
    virtual void setModifyZoneFun(int index, ModifyFunction fun) = 0;
    virtual std::vector<ExtZoneParam*>& getInputControls() = 0;
    virtual std::vector<ExtZoneParam*>& getOutputControls() = 0;
    virtual void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr) = 0;
    virtual void buildUserInterface(UI* ui_interface) = 0;
    virtual void buildUserInterface(UI* ui_interface, char* memory_block) = 0;
    virtual void buildUserInterface(UIGlue* ui_interface, char* memory_block) = 0;
    virtual bool hasCompileOption(const std::string& option) = 0;
};

// Float templated decoder

struct JSONUIFloatDecoder : public JSONUIDecoderReal<float>, public JSONUITemplatedDecoder
{
    JSONUIFloatDecoder(const std::string& json):JSONUIDecoderReal<float>(json)
    {}
    
    void metadata(Meta* m) { JSONUIDecoderReal<float>::metadata(m); }
    void metadata(MetaGlue* glue) { JSONUIDecoderReal<float>::metadata(glue); }
    int getDSPSize() { return fDSPSize; }
    std::string getName() { return fName; }
    std::string getLibVersion() { return fVersion; }
    std::string getCompileOptions() { return fCompileOptions; }
    std::vector<std::string> getLibraryList() { return fLibraryList; }
    std::vector<std::string> getIncludePathnames() { return fIncludePathnames; }
    int getNumInputs() { return fNumInputs; }
    int getNumOutputs() { return fNumOutputs; }
    int getSampleRate(char* memory_block)  { return JSONUIDecoderReal<float>::getSampleRate(memory_block); }
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        JSONUIDecoderReal<float>::setReflectZoneFun(index, fun);
    }
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        JSONUIDecoderReal<float>::setModifyZoneFun(index, fun);
    }
    std::vector<ExtZoneParam*>& getInputControls()
    {
        return fPathInputTable;
    }
    std::vector<ExtZoneParam*>& getOutputControls()
    {
        return fPathOutputTable;
    }
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        JSONUIDecoderReal<float>::resetUserInterface(memory_block, defaultsound);
    }
    void buildUserInterface(UI* ui_interface)
    {
        JSONUIDecoderReal<float>::buildUserInterface(ui_interface);
    }
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        JSONUIDecoderReal<float>::buildUserInterface(ui_interface, memory_block);
    }
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        JSONUIDecoderReal<float>::buildUserInterface(ui_interface, memory_block);
    }
    bool hasCompileOption(const std::string& option) { return JSONUIDecoderReal<float>::hasCompileOption(option); }
};

// Double templated decoder

struct JSONUIDoubleDecoder : public JSONUIDecoderReal<double>, public JSONUITemplatedDecoder
{
    JSONUIDoubleDecoder(const std::string& json):JSONUIDecoderReal<double>(json)
    {}
    
    void metadata(Meta* m) { JSONUIDecoderReal<double>::metadata(m); }
    void metadata(MetaGlue* glue) { JSONUIDecoderReal<double>::metadata(glue); }
    int getDSPSize() { return fDSPSize; }
    std::string getName() { return fName; }
    std::string getLibVersion() { return fVersion; }
    std::string getCompileOptions() { return fCompileOptions; }
    std::vector<std::string> getLibraryList() { return fLibraryList; }
    std::vector<std::string> getIncludePathnames() { return fIncludePathnames; }
    int getNumInputs() { return fNumInputs; }
    int getNumOutputs() { return fNumOutputs; }
    int getSampleRate(char* memory_block) { return JSONUIDecoderReal<double>::getSampleRate(memory_block); }
    void setReflectZoneFun(int index, ReflectFunction fun)
    {
        JSONUIDecoderReal<double>::setReflectZoneFun(index, fun);
    }
    void setModifyZoneFun(int index, ModifyFunction fun)
    {
        JSONUIDecoderReal<double>::setModifyZoneFun(index, fun);
    }
    std::vector<ExtZoneParam*>& getInputControls()
    {
        return fPathInputTable;
    }
    std::vector<ExtZoneParam*>& getOutputControls()
    {
        return fPathOutputTable;
    }
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr)
    {
        JSONUIDecoderReal<double>::resetUserInterface(memory_block, defaultsound);
    }
    void buildUserInterface(UI* ui_interface)
    {
        JSONUIDecoderReal<double>::buildUserInterface(ui_interface);
    }
    void buildUserInterface(UI* ui_interface, char* memory_block)
    {
        JSONUIDecoderReal<double>::buildUserInterface(ui_interface, memory_block);
    }
    void buildUserInterface(UIGlue* ui_interface, char* memory_block)
    {
        JSONUIDecoderReal<double>::buildUserInterface(ui_interface, memory_block);
    }
    bool hasCompileOption(const std::string& option) { return JSONUIDecoderReal<double>::hasCompileOption(option); }
};

// FAUSTFLOAT templated decoder

struct JSONUIDecoder : public JSONUIDecoderReal<FAUSTFLOAT>
{
    JSONUIDecoder(const std::string& json):JSONUIDecoderReal<FAUSTFLOAT>(json)
    {}
};

// Generic factory

static JSONUITemplatedDecoder* createJSONUIDecoder(const std::string& json)
{
    JSONUIDecoder decoder(json);
    if (decoder.hasCompileOption("-double")) {
        return new JSONUIDoubleDecoder(json);
    } else {
        return new JSONUIFloatDecoder(json);
    }
}

#endif
/**************************  END  JSONUIDecoder.h **************************/

//----------------------------------------------------------------
//  Proxy dsp definition created from the DSP JSON description
//  This class allows a 'proxy' dsp to control a real dsp 
//  possibly running somewhere else.
//----------------------------------------------------------------

class proxy_dsp : public dsp {

    private:
    
        JSONUIDecoder* fDecoder;
        int fSampleRate;
        
    public:
    
        proxy_dsp():fDecoder(nullptr), fSampleRate(-1)
        {}
    
        proxy_dsp(const std::string& json)
        {
            init(json);
        }
    
        void init(const std::string& json)
        {
            fDecoder = new JSONUIDecoder(json);
            fSampleRate = -1;
        }
          
        proxy_dsp(dsp* dsp)
        {
            JSONUI builder(dsp->getNumInputs(), dsp->getNumOutputs());
            dsp->metadata(&builder);
            dsp->buildUserInterface(&builder);
            fSampleRate = dsp->getSampleRate();
            fDecoder = new JSONUIDecoder(builder.JSON());
        }
      
        virtual ~proxy_dsp()
        {
            delete fDecoder;
        }
       
        virtual int getNumInputs() { return fDecoder->fNumInputs; }
        virtual int getNumOutputs() { return fDecoder->fNumOutputs; }
        
        virtual void buildUserInterface(UI* ui) { fDecoder->buildUserInterface(ui); }
        
        // To possibly implement in a concrete proxy dsp 
        virtual void init(int sample_rate)
        {
            instanceInit(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            instanceConstants(sample_rate);
            instanceResetUserInterface();
            instanceClear();
        }
        virtual void instanceConstants(int sample_rate) { fSampleRate = sample_rate; }
        virtual void instanceResetUserInterface() { fDecoder->resetUserInterface(); }
        virtual void instanceClear() {}
    
        virtual int getSampleRate() { return fSampleRate; }
    
        virtual proxy_dsp* clone() { return new proxy_dsp(fDecoder->fJSON); }
        virtual void metadata(Meta* m) { fDecoder->metadata(m); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {}
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {} 
        
};

#endif
/**************************  END  proxy-dsp.h **************************/
/************************** BEGIN JSONControl.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JSON_CONTROL__
#define __JSON_CONTROL__

#include <string>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct JSONControl {
    
    virtual std::string getJSON() { return ""; }

    virtual void setParamValue(const std::string& path, FAUSTFLOAT value) {}

    virtual FAUSTFLOAT getParamValue(const std::string& path) { return 0; }
    
    virtual ~JSONControl()
    {}
    
};

#endif
/**************************  END  JSONControl.h **************************/

#define kActiveVoice      0
#define kFreeVoice        -1
#define kReleaseVoice     -2
#define kNoVoice          -3

#define VOICE_STOP_LEVEL  0.0005    // -70 db
#define MIX_BUFFER_SIZE   4096

// endsWith(<str>,<end>) : returns true if <str> ends with <end>

static double midiToFreq(double note)
{
    return 440.0 * std::pow(2.0, (note-69.0)/12.0);
}

/**
 * Allows to control zones in a grouped manner.
 */

class GroupUI : public GUI, public PathBuilder
{

    private:

        std::map<std::string, uiGroupItem*> fLabelZoneMap;

        void insertMap(std::string label, FAUSTFLOAT* zone)
        {
            if (!MapUI::endsWith(label, "/gate")
                && !MapUI::endsWith(label, "/freq")
                && !MapUI::endsWith(label, "/gain")) {

                // Groups all controller except 'freq', 'gate', and 'gain'
                if (fLabelZoneMap.find(label) != fLabelZoneMap.end()) {
                    fLabelZoneMap[label]->addZone(zone);
                } else {
                    fLabelZoneMap[label] = new uiGroupItem(this, zone);
                }
            }
        }

        uiCallbackItem* fPanic;

    public:

        GroupUI(FAUSTFLOAT* zone, uiCallback cb, void* arg)
        {
            fPanic = new uiCallbackItem(this, zone, cb, arg);
        }
    
        virtual ~GroupUI()
        {
            // 'fPanic' is kept and deleted in GUI, so do not delete here
        }

        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }

        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }

        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }

};

/**
 * One voice of polyphony.
 */

struct dsp_voice : public MapUI, public decorator_dsp {

    int fNote;                          // Playing note actual pitch
    int fDate;                          // KeyOn date
    int fRelease;                       // Current number of samples used in release mode to detect end of note
    int fMinRelease;                    // Max of samples used in release mode to detect end of note
    FAUSTFLOAT fLevel;                  // Last audio block level
    std::vector<std::string> fGatePath; // Paths of 'gate' control
    std::vector<std::string> fGainPath; // Paths of 'gain' control
    std::vector<std::string> fFreqPath; // Paths of 'freq' control
 
    dsp_voice(dsp* dsp):decorator_dsp(dsp)
    {
        dsp->buildUserInterface(this);
        fNote = kFreeVoice;
        fLevel = FAUSTFLOAT(0);
        fDate = 0;
        fMinRelease = dsp->getSampleRate()/2; // One 1/2 sec used in release mode to detect end of note
        extractPaths(fGatePath, fFreqPath, fGainPath);
    }
    virtual ~dsp_voice()
    {}

    void extractPaths(std::vector<std::string>& gate, std::vector<std::string>& freq, std::vector<std::string>& gain)
    {
        // Keep gain, freq and gate labels
        std::map<std::string, FAUSTFLOAT*>::iterator it;
        for (it = getMap().begin(); it != getMap().end(); it++) {
            std::string path = (*it).first;
            if (endsWith(path, "/gate")) {
                gate.push_back(path);
            } else if (endsWith(path, "/freq")) {
                freq.push_back(path);
            } else if (endsWith(path, "/gain")) {
                gain.push_back(path);
            }
        }
    }

    // MIDI velocity [0..127]
    void keyOn(int pitch, int velocity, bool trigger)
    {
        keyOn(pitch, float(velocity)/127.f, trigger);
    }

    // Normalized MIDI velocity [0..1]
    void keyOn(int pitch, float velocity, bool trigger)
    {
        // So that DSP state is always re-initialized
        fDSP->instanceClear();
        
        for (size_t i = 0; i < fFreqPath.size(); i++) {
            setParamValue(fFreqPath[i], midiToFreq(pitch));
        }
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(1));
        }
        for (size_t i = 0; i < fGainPath.size(); i++) {
            setParamValue(fGainPath[i], velocity);
        }
        
        fNote = pitch;
    }

    void keyOff(bool hard = false)
    {
        // No use of velocity for now...
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(0));
        }
        
        if (hard) {
            // Immediately stop voice
            fNote = kFreeVoice;
        } else {
            // Release voice
            fRelease = fMinRelease;
            fNote = kReleaseVoice;
        }
    }

};

/**
 * A group of voices.
 */

struct dsp_voice_group {

    GroupUI fGroups;

    std::vector<dsp_voice*> fVoiceTable; // Individual voices
    dsp* fVoiceGroup;                    // Voices group to be used for GUI grouped control

    FAUSTFLOAT fPanic;

    bool fVoiceControl;
    bool fGroupControl;

    dsp_voice_group(uiCallback cb, void* arg, bool control, bool group)
        :fGroups(&fPanic, cb, arg),
        fVoiceGroup(0), fPanic(FAUSTFLOAT(0)),
        fVoiceControl(control), fGroupControl(group)
    {}

    virtual ~dsp_voice_group()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            delete fVoiceTable[i];
        }
        delete fVoiceGroup;
    }

    void addVoice(dsp_voice* voice)
    {
        fVoiceTable.push_back(voice);
    }

    void clearVoices()
    {
        fVoiceTable.clear();
    }

    void init()
    {
        // Groups all uiItem for a given path
        fVoiceGroup = new proxy_dsp(fVoiceTable[0]);
        fVoiceGroup->buildUserInterface(&fGroups);
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->buildUserInterface(&fGroups);
        }
    }
    
    void instanceResetUserInterface()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->instanceResetUserInterface();
        }
    }

    void buildUserInterface(UI* ui_interface)
    {
        if (fVoiceTable.size() > 1) {
            ui_interface->openTabBox("Polyphonic");

            // Grouped voices UI
            ui_interface->openVerticalBox("Voices");
            ui_interface->addButton("Panic", &fPanic);
            fVoiceGroup->buildUserInterface(ui_interface);
            ui_interface->closeBox();

            // If not grouped, also add individual voices UI
            if (!fGroupControl) {
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    char buffer[32];
                    snprintf(buffer, 32, ((fVoiceTable.size() < 8) ? "Voice%ld" : "V%ld"), long(i+1));
                    ui_interface->openHorizontalBox(buffer);
                    fVoiceTable[i]->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                }
            }

            ui_interface->closeBox();
        } else {
            fVoiceTable[0]->buildUserInterface(ui_interface);
        }
    }

};

/**
 * Base class for MIDI controllable DSP.
 */

#ifdef EMCC
#endif

class dsp_poly : public decorator_dsp, public midi, public JSONControl {

    protected:
    
    #ifdef EMCC
        MapUI fMapUI;
        std::string fJSON;
        midi_handler fMIDIHandler;
        MidiUI fMIDIUI;
    #endif
    
    public:
    
    #ifdef EMCC
        dsp_poly(dsp* dsp):decorator_dsp(dsp), fMIDIUI(&fMIDIHandler)
        {
            JSONUI jsonui(getNumInputs(), getNumOutputs());
            buildUserInterface(&jsonui);
            fJSON = jsonui.JSON(true);
            buildUserInterface(&fMapUI);
            buildUserInterface(&fMIDIUI);
        }
    #else
        dsp_poly(dsp* dsp):decorator_dsp(dsp)
        {}
    #endif
    
        virtual ~dsp_poly() {}
    
        // Reimplemented for EMCC
    #ifdef EMCC
        virtual int getNumInputs() { return decorator_dsp::getNumInputs(); }
        virtual int getNumOutputs() { return decorator_dsp::getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { decorator_dsp::buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return decorator_dsp::getSampleRate(); }
        virtual void init(int sample_rate) { decorator_dsp::init(sample_rate); }
        virtual void instanceInit(int sample_rate) { decorator_dsp::instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { decorator_dsp::instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { decorator_dsp::instanceResetUserInterface(); }
        virtual void instanceClear() { decorator_dsp::instanceClear(); }
        virtual dsp_poly* clone() { return new dsp_poly(fDSP->clone()); }
        virtual void metadata(Meta* m) { decorator_dsp::metadata(m); }
    
        // Additional API
        std::string getJSON()
        {
            return fJSON;
        }
    
        virtual void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            fMapUI.setParamValue(path, value);
            GUI::updateAllGuis();
        }
        
        virtual FAUSTFLOAT getParamValue(const std::string& path) { return fMapUI.getParamValue(path); }

        virtual void computeJS(int count, uintptr_t inputs, uintptr_t outputs)
        {
            decorator_dsp::compute(count, reinterpret_cast<FAUSTFLOAT**>(inputs),reinterpret_cast<FAUSTFLOAT**>(outputs));
        }
    #endif
    
        virtual MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return midi::keyOn(channel, pitch, velocity);
        }
        virtual void keyOff(int channel, int pitch, int velocity)
        {
            midi::keyOff(channel, pitch, velocity);
        }
        virtual void keyPress(int channel, int pitch, int press)
        {
            midi::keyPress(channel, pitch, press);
        }
        virtual void chanPress(int channel, int press)
        {
            midi::chanPress(channel, press);
        }
        virtual void ctrlChange(int channel, int ctrl, int value)
        {
            midi::ctrlChange(channel, ctrl, value);
        }
        virtual void ctrlChange14bits(int channel, int ctrl, int value)
        {
            midi::ctrlChange14bits(channel, ctrl, value);
        }
        virtual void pitchWheel(int channel, int wheel)
        {
        #ifdef EMCC
            fMIDIUI.pitchWheel(0., channel, wheel);
            GUI::updateAllGuis();
        #else
            midi::pitchWheel(channel, wheel);
        #endif
        }
        virtual void progChange(int channel, int pgm)
        {
            midi::progChange(channel, pgm);
        }
    
        // Group API
        virtual void setGroup(bool group) {}
        virtual bool getGroup() { return false; }

};

/**
 * Polyphonic DSP: groups a set of DSP to be played together or triggered by MIDI.
 *
 * All voices are preallocated by cloning the single DSP voice given at creation time.
 * Dynamic voice allocation is done in 'getFreeVoice'
 */

class ue_binaural_decoder_poly : public dsp_voice_group, public dsp_poly {

    private:

        FAUSTFLOAT** fMixBuffer;
        int fDate;

        FAUSTFLOAT mixCheckVoice(int count, FAUSTFLOAT** outputBuffer, FAUSTFLOAT** mixBuffer)
        {
            FAUSTFLOAT level = 0;
            for (int i = 0; i < getNumOutputs(); i++) {
                FAUSTFLOAT* mixChannel = mixBuffer[i];
                FAUSTFLOAT* outChannel = outputBuffer[i];
                for (int j = 0; j < count; j++) {
                    level = std::max<FAUSTFLOAT>(level, (FAUSTFLOAT)fabs(outChannel[j]));
                    mixChannel[j] += outChannel[j];
                }
            }
            return level;
        }
    
        void mixVoice(int count, FAUSTFLOAT** outputBuffer, FAUSTFLOAT** mixBuffer)
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                FAUSTFLOAT* mixChannel = mixBuffer[i];
                FAUSTFLOAT* outChannel = outputBuffer[i];
                for (int j = 0; j < count; j++) {
                    mixChannel[j] += outChannel[j];
                }
            }
        }

        void clearOutput(int count, FAUSTFLOAT** mixBuffer)
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                memset(mixBuffer[i], 0, count * sizeof(FAUSTFLOAT));
            }
        }
    
        int getPlayingVoice(int pitch)
        {
            int voice_playing = kNoVoice;
            int oldest_date_playing = INT_MAX;
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fNote == pitch) {
                    // Keeps oldest playing voice
                    if (fVoiceTable[i]->fDate < oldest_date_playing) {
                        oldest_date_playing = fVoiceTable[i]->fDate;
                        voice_playing = int(i);
                    }
                }
            }
            
            return voice_playing;
        }
    
        // Always returns a voice
        int getFreeVoice()
        {
            int voice = kNoVoice;
            
            // Looks for the first available voice
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fNote == kFreeVoice) {
                    voice = int(i);
                    goto result;
                }
            }

            {
                // Otherwise steal one
                int voice_release = kNoVoice;
                int voice_playing = kNoVoice;
                
                int oldest_date_release = INT_MAX;
                int oldest_date_playing = INT_MAX;

                // Scan all voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    if (fVoiceTable[i]->fNote == kReleaseVoice) {
                        // Keeps oldest release voice
                        if (fVoiceTable[i]->fDate < oldest_date_release) {
                            oldest_date_release = fVoiceTable[i]->fDate;
                            voice_release = int(i);
                        }
                    } else {
                        // Otherwise keeps oldest playing voice
                        if (fVoiceTable[i]->fDate < oldest_date_playing) {
                            oldest_date_playing = fVoiceTable[i]->fDate;
                            voice_playing = int(i);
                        }
                    }
                }
            
                // Then decide which one to steal
                if (oldest_date_release != INT_MAX) {
                    std::cout << "Steal release voice : voice_date " << fVoiceTable[voice_release]->fDate << " cur_date = " << fDate << " voice = " << voice_release << std::endl;
                    voice = voice_release;
                    goto result;
                } else if (oldest_date_playing != INT_MAX) {
                    std::cout << "Steal playing voice : voice_date " << fVoiceTable[voice_playing]->fDate << " cur_date = " << fDate << " voice = " << voice_playing << std::endl;
                    voice = voice_playing;
                    goto result;
                } else {
                    assert(false);
                    return kNoVoice;
                }
            }
            
        result:
            fVoiceTable[voice]->fDate = fDate++;
            fVoiceTable[voice]->fNote = kActiveVoice;
            return voice;
        }

        static void panic(FAUSTFLOAT val, void* arg)
        {
            if (val == FAUSTFLOAT(1)) {
                static_cast<ue_binaural_decoder_poly*>(arg)->allNotesOff(true);
            }
        }

        bool checkPolyphony()
        {
            if (fVoiceTable.size() > 0) {
                return true;
            } else {
                std::cout << "DSP is not polyphonic...\n";
                return false;
            }
        }

    public:
    
        /**
         * Constructor.
         *
         * @param dsp - the dsp to be used for one voice. Beware: ue_binaural_decoder_poly will use and finally delete the pointer.
         * @param nvoices - number of polyphony voices, should be at least 1
         * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
         *                If false all voices are always running.
         * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
         *                a given control on all voices, assuming GUI::updateAllGuis() is called.
         *                If false, all voices can be individually controlled.
         *                setGroup/getGroup methods can be used to set/get the group state.
         *
         */
        ue_binaural_decoder_poly(dsp* dsp,
                   int nvoices,
                   bool control = false,
                   bool group = true)
        : dsp_voice_group(panic, this, control, group), dsp_poly(dsp) // dsp parameter is deallocated by ~dsp_poly
        {
            fDate = 0;

            // Create voices
            assert(nvoices > 0);
            for (int i = 0; i < nvoices; i++) {
                addVoice(new dsp_voice(dsp->clone()));
            }

            // Init audio output buffers
            fMixBuffer = new FAUSTFLOAT*[getNumOutputs()];
            for (int i = 0; i < getNumOutputs(); i++) {
                fMixBuffer[i] = new FAUSTFLOAT[MIX_BUFFER_SIZE];
            }

            dsp_voice_group::init();
        }

        virtual ~ue_binaural_decoder_poly()
        {
            for (int i = 0; i < getNumOutputs(); i++) {
                delete[] fMixBuffer[i];
            }
            delete[] fMixBuffer;
        }

        // DSP API
    
        void buildUserInterface(UI* ui_interface)
        {
            dsp_voice_group::buildUserInterface(ui_interface);
        }

        void init(int sample_rate)
        {
            decorator_dsp::init(sample_rate);
            fVoiceGroup->init(sample_rate);
            fPanic = FAUSTFLOAT(0);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->init(sample_rate);
            }
        }
    
        void instanceInit(int samplingFreq)
        {
            instanceConstants(samplingFreq);
            instanceResetUserInterface();
            instanceClear();
        }

        void instanceConstants(int sample_rate)
        {
            decorator_dsp::instanceConstants(sample_rate);
            fVoiceGroup->instanceConstants(sample_rate);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceConstants(sample_rate);
            }
        }

        void instanceResetUserInterface()
        {
            decorator_dsp::instanceResetUserInterface();
            fVoiceGroup->instanceResetUserInterface();
            fPanic = FAUSTFLOAT(0);
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceResetUserInterface();
            }
        }

        void instanceClear()
        {
            decorator_dsp::instanceClear();
            fVoiceGroup->instanceClear();
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceClear();
            }
        }

        virtual ue_binaural_decoder_poly* clone()
        {
            return new ue_binaural_decoder_poly(fDSP->clone(), int(fVoiceTable.size()), fVoiceControl, fGroupControl);
        }

        void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            assert(count <= MIX_BUFFER_SIZE);

            // First clear the outputs
            clearOutput(count, outputs);

            if (fVoiceControl) {
                // Mix all playing voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    dsp_voice* voice = fVoiceTable[i];
                    if (voice->fNote != kFreeVoice) {
                        voice->compute(count, inputs, fMixBuffer);
                        // Mix it in result
                        voice->fLevel = mixCheckVoice(count, fMixBuffer, outputs);
                        // Check the level to possibly set the voice in kFreeVoice again
                        voice->fRelease -= count;
                        if ((voice->fNote == kReleaseVoice)
                            && (voice->fRelease < 0)
                            && (voice->fLevel < VOICE_STOP_LEVEL)) {
                            voice->fNote = kFreeVoice;
                        }
                    }
                }
            } else {
                // Mix all voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    fVoiceTable[i]->compute(count, inputs, fMixBuffer);
                    mixVoice(count, fMixBuffer, outputs);
                }
            }
        }

        void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
    
        // Terminate all active voices, gently or immediately (depending of 'hard' value)
        void allNotesOff(bool hard = false)
        {
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->keyOff(hard);
            }
        }

        // Additional polyphonic API
        MapUI* newVoice()
        {
            int voice = getFreeVoice();
            // So that DSP state is always re-initialized
            fVoiceTable[voice]->instanceClear();
            return fVoiceTable[voice];
        }

        void deleteVoice(MapUI* voice)
        {
            std::vector<dsp_voice*>::iterator it = find(fVoiceTable.begin(), fVoiceTable.end(), reinterpret_cast<dsp_voice*>(voice));
            if (it != fVoiceTable.end()) {
                (*it)->keyOff();
            } else {
                std::cout << "Voice not found\n";
            }
        }
    
        // Group API
        void setGroup(bool group) { fGroupControl = group; }
        bool getGroup() { return fGroupControl; }

        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            if (checkPolyphony()) {
                int voice = getFreeVoice();
                fVoiceTable[voice]->keyOn(pitch, velocity, true);
                return fVoiceTable[voice];
            } else {
                return 0;
            }
        }

        void keyOff(int channel, int pitch, int velocity = 127)
        {
            if (checkPolyphony()) {
                int voice = getPlayingVoice(pitch);
                if (voice != kNoVoice) {
                    fVoiceTable[voice]->keyOff();
                } else {
                    std::cout << "Playing pitch = " << pitch << " not found\n";
                }
            }
        }

        void ctrlChange(int channel, int ctrl, int value)
        {
            if (ctrl == ALL_NOTES_OFF || ctrl == ALL_SOUND_OFF) {
                allNotesOff();
            }
        }

};

/**
 * Polyphonic DSP with an integrated effect. fPolyDSP will respond to MIDI messages.
 */
class dsp_poly_effect : public dsp_poly {
    
    private:
        
        dsp_poly* fPolyDSP;
        
    public:
        
        dsp_poly_effect(dsp_poly* dsp1, dsp* dsp2)
        :dsp_poly(dsp2), fPolyDSP(dsp1)
        {}
        
        virtual ~dsp_poly_effect()
        {
            // dsp_poly_effect is also a decorator_dsp, which will free fPolyDSP
        }
        
        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return fPolyDSP->keyOn(channel, pitch, velocity);
        }
        void keyOff(int channel, int pitch, int velocity)
        {
            fPolyDSP->keyOff(channel, pitch, velocity);
        }
        void keyPress(int channel, int pitch, int press)
        {
            fPolyDSP->keyPress(channel, pitch, press);
        }
        void chanPress(int channel, int press)
        {
            fPolyDSP->chanPress(channel, press);
        }
        void ctrlChange(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange(channel, ctrl, value);
        }
        void ctrlChange14bits(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange14bits(channel, ctrl, value);
        }
        void pitchWheel(int channel, int wheel)
        {
            fPolyDSP->pitchWheel(channel, wheel);
        }
        void progChange(int channel, int pgm)
        {
            fPolyDSP->progChange(channel, pgm);
        }
        
        // Group API
        void setGroup(bool group)
        {
            fPolyDSP->setGroup(group);
        }
        bool getGroup()
        {
            return fPolyDSP->getGroup();
        }
};

/**
 * Polyphonic DSP factory class. Helper code to support polyphonic DSP source with an integrated effect.
 */

struct dsp_poly_factory : public dsp_factory {
    
    dsp_factory* fProcessFactory;
    dsp_factory* fEffectFactory;
    
    std::string getEffectCode(const std::string& dsp_content)
    {
        std::stringstream effect_code;
        effect_code << "adapt(1,1) = _; adapt(2,2) = _,_; adapt(1,2) = _ <: _,_; adapt(2,1) = _,_ :> _;";
        effect_code << "adaptor(F,G) = adapt(outputs(F),inputs(G)); dsp_code = environment{ " << dsp_content << " };";
        effect_code << "process = adaptor(dsp_code.process, dsp_code.effect) : dsp_code.effect;";
        return effect_code.str();
    }

    dsp_poly_factory(dsp_factory* process_factory = NULL,
                     dsp_factory* effect_factory = NULL):
    fProcessFactory(process_factory)
    ,fEffectFactory(effect_factory)
    {}
    
    virtual ~dsp_poly_factory()
    {}
    
    virtual std::string getName() { return fProcessFactory->getName(); }
    virtual std::string getSHAKey() { return fProcessFactory->getSHAKey(); }
    virtual std::string getDSPCode() { return fProcessFactory->getDSPCode(); }
    virtual std::string getCompileOptions() { return fProcessFactory->getCompileOptions(); }
    virtual std::vector<std::string> getLibraryList() { return fProcessFactory->getLibraryList(); }
    virtual std::vector<std::string> getIncludePathnames() { return fProcessFactory->getIncludePathnames(); }
    
    virtual void setMemoryManager(dsp_memory_manager* manager)
    {
        fProcessFactory->setMemoryManager(manager);
        if (fEffectFactory) {
            fEffectFactory->setMemoryManager(manager);
        }
    }
    virtual dsp_memory_manager* getMemoryManager() { return fProcessFactory->getMemoryManager(); }
    
    /* Create a new polyphonic DSP instance with global effect, to be deleted with C++ 'delete'
     *
     * @param nvoices - number of polyphony voices, should be at least 1
     * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
     *                If false all voices are always running.
     * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
     *                a given control on all voices, assuming GUI::updateAllGuis() is called.
     *                If false, all voices can be individually controlled.
     */
    dsp_poly* createPolyDSPInstance(int nvoices, bool control, bool group)
    {
        dsp_poly* dsp_poly = new ue_binaural_decoder_poly(fProcessFactory->createDSPInstance(), nvoices, control, group);
        if (fEffectFactory) {
            // the 'dsp_poly' object has to be controlled with MIDI, so kept separated from new dsp_sequencer(...) object
            return new dsp_poly_effect(dsp_poly, new dsp_sequencer(dsp_poly, fEffectFactory->createDSPInstance()));
        } else {
            return new dsp_poly_effect(dsp_poly, dsp_poly);
        }
    }
    
    /* Create a new DSP instance, to be deleted with C++ 'delete' */
    dsp* createDSPInstance()
    {
        return fProcessFactory->createDSPInstance();
    }
    
};

#endif // __poly_dsp__
/**************************  END  poly-dsp.h **************************/

std::list<GUI*> GUI::fGuiList;
ztimedmap GUI::gTimedZoneMap;

static t_class* faust_class;

/*--------------------------------------------------------------------------*/
static const char* getCodeSize()
{
    int tmp;
    return (sizeof(&tmp) == 8) ? "64 bits" : "32 bits";
}

/*--------------------------------------------------------------------------*/
typedef struct faust
{
    t_pxobject m_ob;
    t_atom *m_seen, *m_want;
    map<string, vector<t_object*> > m_output_table;
    short m_where;
    bool m_mute;
    void** m_args;
    mspUI* m_dspUI;
    dsp* m_dsp;
    ue_binaural_decoder_poly* m_dsp_poly;
    void* m_control_outlet;
    char* m_json;
    t_systhread_mutex m_mutex;    
    int m_Inputs;
    int m_Outputs;
    SaveUI* m_savedUI;
#ifdef MIDICTRL
    MidiUI* m_midiUI;
    midi_handler* m_midiHandler;
#endif
#ifdef SOUNDFILE
    SoundUI* m_soundInterface;
#endif
#ifdef OSCCTRL
    OSCUI* m_oscInterface;
#endif
} t_faust;

void faust_create_jsui(t_faust* x);
void faust_make_json(t_faust* x);

/*--------------------------------------------------------------------------*/
void faust_allocate(t_faust* x, int nvoices)
{
    // Delete old
    delete x->m_dsp;
    x->m_dspUI->clear();
    
    if (nvoices > 0) {
        #ifdef POST
        post("polyphonic DSP voices = %d", nvoices);
        #endif
        x->m_dsp_poly = new ue_binaural_decoder_poly(new ue_binaural_decoder(), nvoices, true, true);
    #ifdef POLY2
        x->m_dsp = new dsp_sequencer(x->m_dsp_poly, new effect());
    #else
        x->m_dsp = x->m_dsp_poly;
    #endif
    #ifdef MIDICTRL
        x->m_midiHandler->addMidiIn(x->m_dsp_poly);
    #endif
    } else {
        #ifdef POST
        post("monophonic DSP");
        #endif
    #if (DOWN_SAMPLING > 0)
        #if (FILTER_TYPE == 0)
            x->m_dsp = new dsp_down_sampler<Identity<Double<1,1>, DOWN_SAMPLING>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 1)
            x->m_dsp = new dsp_down_sampler<LowPass3<Double<45,100>, DOWN_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 2)
            x->m_dsp = new dsp_down_sampler<LowPass4<Double<45,100>, DOWN_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 3)
            x->m_dsp = new dsp_down_sampler<LowPass3e<Double<45,100>, DOWN_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 4)
            x->m_dsp = new dsp_down_sampler<LowPass6eé<Double<45,100>, DOWN_SAMPLING, double>>(new ue_binaural_decoder());
        #else
            #error "ERROR : Filter type must be in [0..4] range"
        #endif
    #elif (UP_SAMPLING > 0)
        #if (FILTER_TYPE == 0)
            x->m_dsp = new dsp_up_sampler<Identity<Double<1,1>, UP_SAMPLING>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 1)
            x->m_dsp = new dsp_up_sampler<LowPass3<Double<45,100>, UP_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 2)
            x->m_dsp = new dsp_up_sampler<LowPass4<Double<45,100>, UP_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 3)
            x->m_dsp = new dsp_up_sampler<LowPass3e<Double<45,100>, UP_SAMPLING, double>>(new ue_binaural_decoder());
        #elif (FILTER_TYPE == 4)
            x->m_dsp = new dsp_up_sampler<LowPass6e<Double<45,100>, UP_SAMPLING, double>>(new ue_binaural_decoder());
        #else
            #error "ERROR : Filter type must be in [0..4] range"
        #endif
    #else
        x->m_dsp = new ue_binaural_decoder();
    #endif
    }
    
#ifdef MIDICTRL
    x->m_dsp->buildUserInterface(x->m_midiUI);
#endif
  
    // Possible sample adaptation
    if (sizeof(FAUSTFLOAT) == 4) {
        x->m_dsp = new dsp_sample_adapter<FAUSTFLOAT, double>(x->m_dsp);
    }
}

/*--------------------------------------------------------------------------*/
void faust_anything(t_faust* obj, t_symbol* s, short ac, t_atom* av)
{
    bool res = false;
    string name = string((s)->s_name);
    
    // If no argument is there, consider it as a toggle message for a button
    if (ac == 0 && obj->m_dspUI->isValue(name)) {
        
        FAUSTFLOAT off = FAUSTFLOAT(0.0);
        FAUSTFLOAT on = FAUSTFLOAT(1.0);
        obj->m_dspUI->setValue(name, off);
        obj->m_dspUI->setValue(name, on);
        
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = off;
        faust_anything(obj, s, 1, av);
        
    } else if (mspUI::checkDigit(name)) { // List of values
        
        int pos, ndigit = 0;
        for (pos = name.size() - 1; pos >= 0; pos--) {
            if (isdigit(name[pos]) || name[pos] == ' ') {
                ndigit++;
            } else {
                break;
            }
        }
        pos++;
        
        string prefix = name.substr(0, pos);
        string num_base = name.substr(pos);
        int num = atoi(num_base.c_str());
        
        int i;
        t_atom* ap;
       
        // Increment ap each time to get to the next atom
        for (i = 0, ap = av; i < ac; i++, ap++) {
            FAUSTFLOAT value;
            switch (atom_gettype(ap)) {
                case A_LONG:
                    value = FAUSTFLOAT(ap[0].a_w.w_long);
                    break;
                case A_FLOAT:
                    value = FAUSTFLOAT(ap[0].a_w.w_float);
                    break;
                default:
                    post("Invalid argument in parameter setting"); 
                    return;         
            }
            
            string num_val = to_string(num + i);
            stringstream param_name; param_name << prefix;
            for (int i = 0; i < ndigit - mspUI::countDigit(num_val); i++) {
                param_name << ' ';
            }
            param_name << num_val;
              
            // Try special naming scheme for list of parameters
            res = obj->m_dspUI->setValue(param_name.str(), value);
            
            // Otherwise try standard name
            if (!res) {
                res = obj->m_dspUI->setValue(name, value);
            }
            if (!res) {
                post("Unknown parameter : %s", (s)->s_name);
            }
        }
        
    } else {
        // Standard parameter name
        FAUSTFLOAT value = (av[0].a_type == A_LONG) ? FAUSTFLOAT(av[0].a_w.w_long) : FAUSTFLOAT(av[0].a_w.w_float);
        res = obj->m_dspUI->setValue(name, value);
        if (!res) {
            post("Unknown parameter : %s", (s)->s_name);
        }
    }
}

/*--------------------------------------------------------------------------*/
void faust_polyphony(t_faust* x, t_symbol* s, short ac, t_atom* av)
{
    if (systhread_mutex_lock(x->m_mutex) == MAX_ERR_NONE) {
        
    #ifdef MIDICTRL
        if (x->m_dsp_poly) {
            x->m_midiHandler->removeMidiIn(x->m_dsp_poly);
        }
    #endif
        
        faust_allocate(x, av[0].a_w.w_long);
        
        // Initialize at the system's sampling rate
        x->m_dsp->init(long(sys_getsr()));
        // Initialize User Interface (here connnection with controls)
        x->m_dsp->buildUserInterface(x->m_dspUI);
        
        // Prepare JSON
        faust_make_json(x);
        
        // Send JSON to JS script
        faust_create_jsui(x);
        
        // Load old controller state
        x->m_dsp->buildUserInterface(x->m_savedUI);
        
        systhread_mutex_unlock(x->m_mutex);
    } else {
        post("Mutex lock cannot be taken...");
    }
}

/*--------------------------------------------------------------------------*/
#ifdef MIDICTRL
void faust_midievent(t_faust* x, t_symbol* s, short ac, t_atom* av) 
{
    if (ac > 0) {
        int type = (int)av[0].a_w.w_long & 0xf0;
        int channel = (int)av[0].a_w.w_long & 0x0f;
                
        if (ac == 1) {
            x->m_midiHandler->handleSync(0.0, av[0].a_w.w_long);
        } else if (ac == 2) {
            x->m_midiHandler->handleData1(0.0, type, channel, av[1].a_w.w_long);
        } else if (ac == 3) {
            x->m_midiHandler->handleData2(0.0, type, channel, av[1].a_w.w_long, av[2].a_w.w_long);
        }
    }
}
#endif

/*--------------------------------------------------------------------------*/
void faust_create_jsui(t_faust* x)
{
    t_object *patcher, *box, *obj;
    object_obex_lookup((t_object*)x, gensym("#P"), &patcher);
    
    for (box = jpatcher_get_firstobject(patcher); box; box = jbox_get_nextobject(box)) {
        obj = jbox_get_object(box);
        // Notify JSON
        if (obj && strcmp(object_classname(obj)->s_name, "js") == 0) {
            t_atom json;
            atom_setsym(&json, gensym(x->m_json));
            object_method_typed(obj, gensym("anything"), 1, &json, 0);
        }
    }
        
    // Keep all outputs to be notified in update_outputs
    x->m_output_table.clear();
    for (box = jpatcher_get_firstobject(patcher); box; box = jbox_get_nextobject(box)) {
        obj = jbox_get_object(box);
        t_symbol* scriptingname = jbox_get_varname(obj); // scripting name
        // Keep control outputs
        if (scriptingname && x->m_dspUI->isOutputValue(scriptingname->s_name)) {
            x->m_output_table[scriptingname->s_name].push_back(obj);
        }
    }
}

/*--------------------------------------------------------------------------*/
void faust_update_outputs(t_faust* x)
{
    for (auto& it1 : x->m_output_table) {
        bool new_val = false;
        FAUSTFLOAT value = x->m_dspUI->getOutputValue(it1.first, new_val);
        if (new_val) {
            t_atom at_value;
            atom_setfloat(&at_value, value);
            for (auto& it2 : it1.second) {
                object_method_typed(it2, gensym("float"), 1, &at_value, 0);
            }
        }
    }
}

/*--------------------------------------------------------------------------*/
void faust_make_json(t_faust* x)
{
    // Prepare JSON
    if (x->m_json) free(x->m_json);
    JSONUI builder(x->m_dsp->getNumInputs(), x->m_dsp->getNumOutputs());
    x->m_dsp->metadata(&builder);
    x->m_dsp->buildUserInterface(&builder);
    x->m_json = strdup(builder.JSON().c_str());
}

/*--------------------------------------------------------------------------*/
void* faust_new(t_symbol* s, short ac, t_atom* av)
{
    bool midi_sync = false;
    int nvoices = 0;
    
    ue_binaural_decoder* tmp_dsp = new ue_binaural_decoder();
    MidiMeta::analyse(tmp_dsp, midi_sync, nvoices);
    delete tmp_dsp;
    
    t_faust* x = (t_faust*)object_alloc(faust_class);
    
    x->m_savedUI = new SaveLabelUI();
    x->m_dspUI = NULL;
    x->m_dsp = NULL;
    x->m_dsp_poly = NULL;
    x->m_json = NULL;
    x->m_mute = false;
    
#ifdef MIDICTRL
    x->m_midiHandler = new midi_handler();
    x->m_midiUI = new MidiUI(x->m_midiHandler);
#endif
    
    x->m_dspUI = new mspUI();
    
    faust_allocate(x, nvoices);
    
    x->m_Inputs = x->m_dsp->getNumInputs();
    x->m_Outputs = x->m_dsp->getNumOutputs();
    
    x->m_control_outlet = outlet_new((t_pxobject*)x, (char*)"list");

    // Initialize at the system's sampling rate
    x->m_dsp->init(long(sys_getsr()));
    // Initialize User Interface (here connnection with controls)
    x->m_dsp->buildUserInterface(x->m_dspUI);
    
    t_max_err err = systhread_mutex_new(&x->m_mutex, SYSTHREAD_MUTEX_NORMAL);
    if (err != MAX_ERR_NONE) {
        post("Cannot allocate mutex...");
    }
    
    // Prepare JSON
    faust_make_json(x);
    
    int num_input;
    if (x->m_dspUI->isMulti()) {
        num_input = x->m_dsp->getNumInputs() + 1;
    } else {
        num_input = x->m_dsp->getNumInputs();
    }
    
    x->m_args = (void**)calloc((num_input + x->m_dsp->getNumOutputs()) + 2, sizeof(void*));
    /* Multi in */
    dsp_setup((t_pxobject*)x, num_input);

    /* Multi out */
    for (int i = 0; i < x->m_dsp->getNumOutputs(); i++) {
        outlet_new((t_pxobject*)x, (char*)"signal");
    }

    ((t_pxobject*)x)->z_misc = Z_NO_INPLACE; // To assure input and output buffers are actually different

#ifdef SOUNDFILE
    Max_Meta3 meta3;
    x->m_dsp->metadata(&meta3);
    string bundle_path_str = SoundUI::getBinaryPathFrom(meta3.fName);
    if (bundle_path_str == "") {
        post("Bundle_path '%s' cannot be found!", meta3.fName.c_str());
    }
    x->m_soundInterface = new SoundUI(bundle_path_str);
    // SoundUI has to be dispatched on all internal voices
    if (x->m_dsp_poly) x->m_dsp_poly->setGroup(false);
    x->m_dsp->buildUserInterface(x->m_soundInterface);
    if (x->m_dsp_poly) x->m_dsp_poly->setGroup(true);
#endif
    
#ifdef OSCCTRL
    x->m_oscInterface = NULL;
#endif
    
    // Send JSON to JS script
    faust_create_jsui(x);
    
    // Load old controller state
    x->m_dsp->buildUserInterface(x->m_savedUI);
    
    // Display controls
#ifdef POST
    x->m_dspUI->displayControls();
#endif   
    // Get attributes values
    attr_args_process(x, ac, av);
    
    return x;
}

#ifdef OSCCTRL
// osc 'IP inport outport xmit bundle'
/*--------------------------------------------------------------------------*/
void faust_osc(t_faust* x, t_symbol* s, short ac, t_atom* av)
{
    if (ac == 5) {
        if (systhread_mutex_lock(x->m_mutex) == MAX_ERR_NONE) {
            
            delete x->m_oscInterface;
            
            const char* argv1[32];
            int argc1 = 0;
            
            argv1[argc1++] = "Faust";
            
            argv1[argc1++]  = "-desthost";
            argv1[argc1++]  = atom_getsym(&av[0])->s_name;
            
            char inport[32];
            snprintf(inport, 32, "%ld", long(av[1].a_w.w_long));
            argv1[argc1++] = "-port";
            argv1[argc1++] = inport;
            
            char outport[32];
            snprintf(outport, 32, "%ld", long(av[2].a_w.w_long));
            argv1[argc1++] = "-outport";
            argv1[argc1++] = outport;
            
            char xmit[32];
            snprintf(xmit, 32, "%ld", long(av[3].a_w.w_long));
            argv1[argc1++] = "-xmit";
            argv1[argc1++] = xmit;
            
            char bundle[32];
            snprintf(bundle, 32, "%ld", long(av[4].a_w.w_long));
            argv1[argc1++] = "-bundle";
            argv1[argc1++] = bundle;
            
            x->m_oscInterface = new OSCUI("Faust", argc1, (char**)argv1);
            x->m_dsp->buildUserInterface(x->m_oscInterface);
            x->m_oscInterface->run();

            post(x->m_oscInterface->getInfos().c_str());

            systhread_mutex_unlock(x->m_mutex);
        } else {
            post("Mutex lock cannot be taken...");
        }
    } else {
        post("Should be : osc 'IP inport outport xmit(0|1|2) bundle(0|1)'");
    }
}
#endif

/*--------------------------------------------------------------------------*/
// Reset controllers to init value and send [path, init, min, max]
void faust_init(t_faust* x, t_symbol* s, short ac, t_atom* av)
{
    // Reset internal state
    x->m_savedUI->reset();
    
    // Input controllers
    for (mspUI::iterator it = x->m_dspUI->begin2(); it != x->m_dspUI->end2(); it++) {
        t_atom myList[4];
        atom_setsym(&myList[0], gensym((*it).first.c_str()));
        atom_setfloat(&myList[1], (*it).second->getInitValue());    // init value
        atom_setfloat(&myList[2], (*it).second->getMinValue());
        atom_setfloat(&myList[3], (*it).second->getMaxValue());
        outlet_list(x->m_control_outlet, 0, 4, myList);
    }
    // Output controllers
    for (mspUI::iterator it = x->m_dspUI->begin4(); it != x->m_dspUI->end4(); it++) {
        t_atom myList[4];
        atom_setsym(&myList[0], gensym((*it).first.c_str()));
        atom_setfloat(&myList[1], (*it).second->getInitValue());    // init value
        atom_setfloat(&myList[2], (*it).second->getMinValue());
        atom_setfloat(&myList[3], (*it).second->getMaxValue());
        outlet_list(x->m_control_outlet, 0, 4, myList);
    }
 }

/*--------------------------------------------------------------------------*/
// Dump controllers as list of: [path, cur, init, min, max]
void faust_dump(t_faust* x, t_symbol* s, short ac, t_atom* av)
{
    // Input controllers
    for (mspUI::iterator it = x->m_dspUI->begin2(); it != x->m_dspUI->end2(); it++) {
        t_atom myList[4];
        atom_setsym(&myList[0], gensym((*it).first.c_str()));
        atom_setfloat(&myList[1], (*it).second->getValue());    // cur value
        atom_setfloat(&myList[2], (*it).second->getMinValue());
        atom_setfloat(&myList[3], (*it).second->getMaxValue());
        outlet_list(x->m_control_outlet, 0, 4, myList);
    }
    // Output controllers
    for (mspUI::iterator it = x->m_dspUI->begin4(); it != x->m_dspUI->end4(); it++) {
        t_atom myList[4];
        atom_setsym(&myList[0], gensym((*it).first.c_str()));
        atom_setfloat(&myList[1], (*it).second->getValue());    // cur value
        atom_setfloat(&myList[2], (*it).second->getMinValue());
        atom_setfloat(&myList[3], (*it).second->getMaxValue());
        outlet_list(x->m_control_outlet, 0, 4, myList);
    }
}

/*--------------------------------------------------------------------------*/
void faust_dblclick(t_faust* x, long inlet)
{
    x->m_dspUI->displayControls();
}

/*--------------------------------------------------------------------------*/
//11/13/2015 : faust_assist is actually called at each click in the patcher, so we now use 'faust_dblclick' to display the parameters...
void faust_assist(t_faust* x, void* b, long msg, long a, char* dst)
{
    if (msg == ASSIST_INLET) {
        if (a == 0) {
            if (x->m_dsp->getNumInputs() == 0) {
                sprintf(dst, "(message) : Unused Input");
            } else {
                sprintf(dst, "(signal) : Audio Input %ld", (a+1));
            }
        } else if (a < x->m_dsp->getNumInputs()) {
            sprintf(dst, "(signal) : Audio Input %ld", (a+1));
        }
    } else if (msg == ASSIST_OUTLET) {
        if (a < x->m_dsp->getNumOutputs()) {
            sprintf(dst, "(signal) : Audio Output %ld", (a+1));
        } else {
            sprintf(dst, "(list) : [path, cur|init, min, max]*");
        }
    }
}

/*--------------------------------------------------------------------------*/
void faust_mute(t_faust* obj, t_symbol* s, short ac, t_atom* at)
{
    if (atom_gettype(at) == A_LONG) {
        obj->m_mute = atom_getlong(at);
    }
}

/*--------------------------------------------------------------------------*/
void faust_free(t_faust* x)
{
    dsp_free((t_pxobject*)x);
    delete x->m_dsp;
    delete x->m_dspUI;
    delete x->m_savedUI;
    if (x->m_args) free(x->m_args);
    if (x->m_json) free(x->m_json);
    systhread_mutex_free(x->m_mutex);
#ifdef MIDICTRL
    // m_midiUI *must* be deleted before m_midiHandler
    delete x->m_midiUI;
    delete x->m_midiHandler;
#endif
#ifdef SOUNDFILE
    delete x->m_soundInterface;
#endif
#ifdef OSCCTRL
    delete x->m_oscInterface;
#endif
}

/*--------------------------------------------------------------------------*/
void faust_perform64(t_faust* x, t_object* dsp64, double** ins, long numins, double** outs, long numouts, long sampleframes, long flags, void* userparam)
{
    AVOIDDENORMALS;
    if (!x->m_mute && systhread_mutex_trylock(x->m_mutex) == MAX_ERR_NONE) {
        if (x->m_dsp) {
            if (x->m_dspUI->isMulti()) {
                x->m_dspUI->setMultiValues(reinterpret_cast<FAUSTFLOAT*>(ins[0]), sampleframes);
                x->m_dsp->compute(sampleframes, reinterpret_cast<FAUSTFLOAT**>(++ins), reinterpret_cast<FAUSTFLOAT**>(outs));
            } else {
                x->m_dsp->compute(sampleframes, reinterpret_cast<FAUSTFLOAT**>(ins), reinterpret_cast<FAUSTFLOAT**>(outs));
            }
        #ifdef OSCCTRL
            if (x->m_oscInterface) x->m_oscInterface->endBundle();
        #endif
            faust_update_outputs(x);
        }
    #if defined(MIDICTRL) || defined(OSCCTRL)
        GUI::updateAllGuis();
    #endif
        systhread_mutex_unlock(x->m_mutex);
    } else {
        // Write null buffers to outs
        for (int i = 0; i < numouts; i++) {
             memset(outs[i], 0, sizeof(double) * sampleframes);
        }
    }
}

/*--------------------------------------------------------------------------*/
void faust_dsp64(t_faust* x, t_object* dsp64, short* count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, faust_perform64, 0, NULL);
}

/*--------------------------------------------------------------------------*/
t_max_err faust_attr_set(t_faust* x, t_object* attr, long ac, t_atom* av)
{
    if (ac && av) {
        t_symbol* attrname = (t_symbol*)object_method(attr, gensym("getname"));
        // Redirect on the generic message handling method
        faust_anything(x, attrname, ac, av);
    }
    return MAX_ERR_NONE;
}

/*--------------------------------------------------------------------------*/
#ifdef _WIN32
extern "C" int main(void)
#else
void ext_main(void* r)
#endif
{
    string file_name = string(FAUST_FILE_NAME);
    // Remove ".dsp" ending
    string class_name = file_name.erase(file_name.size()-4) + "~";
    t_class* c = class_new(class_name.c_str(), (method)faust_new, (method)faust_free, sizeof(t_faust), 0L, A_GIMME, 0);
    
    class_addmethod(c, (method)faust_anything, "anything", A_GIMME, 0);
    class_addmethod(c, (method)faust_polyphony, "polyphony", A_GIMME, 0);
#ifdef OSCCTRL
    class_addmethod(c, (method)faust_osc, "osc", A_GIMME, 0);
#endif
    class_addmethod(c, (method)faust_init, "init", A_GIMME, 0);
    class_addmethod(c, (method)faust_dump, "dump", A_GIMME, 0);
#ifdef MIDICTRL
    class_addmethod(c, (method)faust_midievent, "midievent", A_GIMME, 0);
#endif
    class_addmethod(c, (method)faust_dsp64, "dsp64", A_CANT, 0);
    class_addmethod(c, (method)faust_dblclick, "dblclick", A_CANT, 0);
    class_addmethod(c, (method)faust_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)faust_mute, "mute", A_GIMME, 0);
    
    dsp* tmp_dsp = new ue_binaural_decoder();
    mspUI tmp_UI;
    tmp_dsp->buildUserInterface(&tmp_UI);
    
    // Setup attributes
    int i = 0;
    if (sizeof(FAUSTFLOAT) == 4) {
        for (mspUI::iterator it = tmp_UI.begin1(); it != tmp_UI.end1(); it++, i++) {
            CLASS_ATTR_FLOAT(c, (*it).first.c_str(), 0, t_faust, m_ob);
            CLASS_ATTR_ACCESSORS(c, (*it).first.c_str(), NULL, (method)faust_attr_set);
        }
    } else {
        for (mspUI::iterator it = tmp_UI.begin1(); it != tmp_UI.end1(); it++, i++) {
            CLASS_ATTR_DOUBLE(c, (*it).first.c_str(), 0, t_faust, m_ob);
            CLASS_ATTR_ACCESSORS(c, (*it).first.c_str(), NULL, (method)faust_attr_set);
        }
    }
    
    class_dspinit(c);
    class_register(CLASS_BOX, c);
    faust_class = c;
#ifdef POST
    post((char*)"Faust DSP object v%s (sample = %s bits code = %s)", EXTERNAL_VERSION, ((sizeof(FAUSTFLOAT) == 4) ? "32" : "64"), getCodeSize());
    post((char*)"Copyright (c) 2012-2020 Grame");
#endif
    Max_Meta1 meta1;
    tmp_dsp->metadata(&meta1);
    if (meta1.fCount > 0) {
        #ifdef POST
        Max_Meta2 meta2;
        post("------------------------------");
        tmp_dsp->metadata(&meta2);
        post("------------------------------");
        #endif
    }
    delete(tmp_dsp);
#ifdef _WIN32
    return 0;
#endif
}

/********************END ARCHITECTURE SECTION (part 2/2)****************/


#endif
