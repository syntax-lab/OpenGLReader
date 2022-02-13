#pragma once

#ifndef EXPANDABLE_ENUM_H
    #define EXPANDABLE_ENUM_H
    namespace expandable_enum{
        class enum_status_error{
            int error_status;
        public:
            constexpr enum_status_error(int new_error_status) noexcept: error_status{new_error_status}{}
            constexpr operator int() const noexcept { return error_status; }
        };
        constexpr enum_status_error NO_ERROR = 0;
    }
#endif /* EXPANDABLE_ENUM_H */
