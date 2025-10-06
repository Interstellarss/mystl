#ifndef MYSTL_CONFIG_H
#define MYSTL_CONFIG_H


// Define the namespace for the custom STL implementation

#define MYSTL_VERSION "0.1.0"

namespace mystl {

    inline constexpr const char* version() {
        return MYSTL_VERSION;
    }


} // namespace mystl


#endif // MYSTL_CONFIG_H