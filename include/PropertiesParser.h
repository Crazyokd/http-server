/*
 * Copyright (c) 2014 Fredy Wijaya
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PROPERTIESPARSER_H_
#define PROPERTIESPARSER_H_

#include <string>
#include <vector>
#include <map>
#include <exception>

namespace cppproperties {

class PropertiesException : public std::exception {
public:
    PropertiesException() {}
    PropertiesException(const std::string& msg) throw() : message(msg) {}

    virtual ~PropertiesException() throw() {}

    const std::string& str() const throw() { return message; }

    virtual const char* what() const throw() { return message.c_str(); }

private:
    std::string message;
};

class PropertyNotFoundException : public std::exception {
public:
    PropertyNotFoundException() {}
    PropertyNotFoundException(const std::string& msg) throw() : message(msg) {}

    virtual ~PropertyNotFoundException() throw() {}

    const std::string& str() const throw() { return message; }

    virtual const char* what() const throw() { return message.c_str(); }

private:
    std::string message;
};

class Properties {
public:
    Properties();
    virtual ~Properties();

    /**
     * Gets the property value from a given key.
     *
     * This method throws a PropertyNotFoundException when a given key does not
     * exist.
     */
    std::string GetProperty(const std::string& key) const;

    /**
     * Gets the property value from a given key. Use a default value if not found.
     */
    std::string GetProperty(const std::string& key, const std::string& defaultValue) const;

    /**
     * Gets the list of property names.
     */
    std::vector<std::string> GetPropertyNames() const;

    /**
     * Adds a new property. If the property already exists, it'll overwrite
     * the old one.
     */
    void AddProperty(const std::string& key, const std::string& value);

    /**
     * Removes the property from a given key.
     *
     * If the property doesn't exist a PropertyNotFoundException will be thrown.
     */
    void RemoveProperty(const std::string& key);
private:
    // to preserve the order
    std::vector<std::string> keys;
    std::map<std::string, std::string> properties;
};

class PropertiesParser {
public:
    PropertiesParser();
    virtual ~PropertiesParser();

    /**
     * Reads a properties file and returns a Properties object.
     */
    static Properties Read(const std::string& file);

    /**
     * Writes Properties object to a file.
     */
    static void Write(const std::string& file, const Properties& props);
};

} /* namespace cppproperties */

#endif /* PROPERTIESPARSER_H_ */