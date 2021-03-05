#ifndef BIT_UTILS
#define BIT_UTILS
#include <bitset>
#include <iostream>
#include <type_traits>

namespace bit_utils
{

template<size_t num_bits> std::bitset<num_bits> flip_endian(const std::bitset<num_bits> &bits)
{
    std::bitset<num_bits> result;
    for(int bit_index = 0; bit_index < num_bits; ++bit_index)
    {
        result[bit_index] = bits[num_bits-bit_index-1];
    }
    return result;
}

template<size_t start_pos, size_t end_pos, size_t num_bits> std::bitset<end_pos-start_pos> subset(const std::bitset<num_bits> &bits)
{
    std::bitset<end_pos-start_pos> sub_bits;
    for(int index = 0; index < sub_bits.size(); ++index)
    {
        sub_bits[index] = bits[index+start_pos];
    }
    return sub_bits;
}

template<typename T, typename R> T twos_complement(R bits, int length)
{
    size_t T_size = sizeof(T)*8;
    return R(bits | (((1UL<<(T_size-length))-1)<<length));
}

template <class T, class R> T get_msb_bits(const R* buffer, int position, int length = sizeof(T))
{
    size_t T_size = sizeof(T)*8;
    size_t R_size = sizeof(R)*8;

    T bits = 0;
    
    for(int bit_index = 0; bit_index < length; ++bit_index)
    {
        int word_index = (position+bit_index)/R_size;
        int bit_in_word_index = (position+bit_index)%R_size;
        int shift_factor = R_size - bit_in_word_index - 1;
        unsigned long bit_value = ((buffer[word_index] & (1UL<<shift_factor))>>shift_factor);
        bits |= (bit_value & 1UL) << (length-bit_index-1);
    }

    if(std::is_signed<T>::value && (bits & (1UL<<(length-1))) && T_size > length)
    {
        bits = twos_complement<T>(bits, length);
    }
    
    return bits;
}

template <class T, class R> T get_lsb_bits(const R* buffer, int position, int length = sizeof(T))
{
    size_t T_size = sizeof(T)*8;
    size_t R_size = sizeof(R)*8;

    T bits = 0;

    for(int bit_index = 0; bit_index < length; ++bit_index)
    {
        int word_index = (position+bit_index)/R_size;
        int bit_in_word_index = (position+bit_index)%R_size;
        int shift_factor = bit_in_word_index;
        unsigned long bit_value = ((buffer[word_index] & (1UL<<shift_factor))>>shift_factor);
        bits |= (bit_value & 1UL) << (bit_index);
    }

    if(std::is_signed<T>::value && (bits & (1UL<<(length-1))) && T_size > length)
    {
        bits = twos_complement<T>(bits, length);
    }

    return bits;
}

template <typename T, class R> T splice_msb_bits(const R* buffer, int start_1, int len_1, int start_2, int len_2)
{
    typedef typename std::make_unsigned<T>::type unsigned_T;
    T bits = 0;

    auto unsigned_bits = (get_msb_bits<unsigned_T>(buffer, start_1, len_1)) << len_2;
    unsigned_bits |= get_msb_bits<unsigned_T>(buffer, start_2, len_2);

    if(std::is_signed<T>::value)
    {
        bits = twos_complement<T>(unsigned_bits, len_1+len_2);
    }
    else
    {
        bits = unsigned_bits;
    }

    return bits;
}

}
#endif