#include "byte_buffer_ref.h"

namespace mini {

//
// constructors.
//

template <
  typename T
>
buffer_ref<T>::buffer_ref(
  void
  )
  : _begin(nullptr)
  , _end(nullptr)
{

}

template <
  typename T
>
buffer_ref<T>::buffer_ref(
  const buffer_ref& other
  )
  : _begin(other._begin)
  , _end(other._end)
{

}

template <
  typename T
>
buffer_ref<T>::buffer_ref(
  buffer_ref&& other
  ) : buffer_ref()
{
  swap(other);
}

template <
  typename T
>
buffer_ref<T>::buffer_ref(
  std::nullptr_t
  ) : buffer_ref()
{

}

template <
  typename T
>
buffer_ref<T>::buffer_ref(
  std::initializer_list<T> values
  )
  : _begin((value_type*)values.begin())
  , _end((value_type*)values.end())
{

}

template <
  typename T
>
template <
  typename CONST_ITERATOR_TYPE
>
buffer_ref<T>::buffer_ref(
  CONST_ITERATOR_TYPE begin,
  CONST_ITERATOR_TYPE end
  )
  : _begin((T*)begin)
  , _end((T*)end)
{

}

template <
  typename T
>
template <
  typename U,
  size_type N
>
buffer_ref<T>::buffer_ref(
  const U (&array)[N]
  )
  : _begin((T*)array)
  , _end((T*)array + N)
{

}

//
// assign operators.
//

template <
  typename T
>
buffer_ref<T>&
buffer_ref<T>::operator=(
  const buffer_ref& other
  )
{
  _begin = other._begin;
  _end = other._end;

  return *this;
}

template <
  typename T
>
buffer_ref<T>&
buffer_ref<T>::operator=(
  buffer_ref&& other
  )
{
  swap(other);

  return *this;
}

//
// swap.
//

template <
  typename T
>
void
buffer_ref<T>::swap(
  buffer_ref& other
  )
{
  mini::swap(_begin, other._begin);
  mini::swap(_end, other._end);
}

//
// element access.
//

template <
  typename T
>
typename buffer_ref<T>::const_reference
buffer_ref<T>::operator[](
  size_type index
  ) const
{
  return at(index);
}

template <
  typename T
>
typename buffer_ref<T>::const_reference
buffer_ref<T>::at(
  size_type index
  ) const
{
  return _begin[index];
}

template <
  typename T
>
const typename buffer_ref<T>::value_type*
buffer_ref<T>::get_buffer(
  void
  ) const
{
  return _begin;
}

//
// iterators.
//

template <
  typename T
>
typename buffer_ref<T>::const_iterator
buffer_ref<T>::begin(
  void
  ) const
{
  return _begin;
}

template <
  typename T
>
typename buffer_ref<T>::const_iterator
buffer_ref<T>::end(
  void
  ) const
{
  return _end;
}

//
// capacity.
//

template <
  typename T
>
bool
buffer_ref<T>::is_empty(
  void
  ) const
{
  return get_size() == 0;
}

template <
  typename T
>
typename buffer_ref<T>::size_type
buffer_ref<T>::get_size(
  void
  ) const
{
  return _end - _begin;
}

//
// pointer arithmetic.
//

template <
  typename T
>
typename buffer_ref<T>::const_pointer
buffer_ref<T>::operator++(
  void
  )
{
  return ++_begin;
}

template <
  typename T
>
typename buffer_ref<T>::const_pointer
buffer_ref<T>::operator++(
  int
  )
{
  return _begin++;
}

template <
  typename T
>
typename buffer_ref<T>::const_pointer
buffer_ref<T>::operator--(
  void
  )
{
  return --_begin;
}

template <
  typename T
>
typename buffer_ref<T>::const_pointer
buffer_ref<T>::operator--(
  int
  )
{
  return _begin--;
}

template <
  typename T
>
buffer_ref<T>
buffer_ref<T>::operator+(
  size_t rhs
  ) const
{
  buffer_ref<T> result = *this;
  result._begin += rhs;

  return result;
}

template <
  typename T
>
buffer_ref<T>
buffer_ref<T>::operator-(
  size_t rhs
  ) const
{
  buffer_ref<T> result = *this;
  result._begin -= rhs;

  return result;
}

//
// operations.
//

template <
  typename T
>
bool
buffer_ref<T>::equals(
  const buffer_ref other
  ) const
{
  return
    get_size() == other.get_size() &&
    compare(other) == 0;
}

template <
  typename T
>
int
buffer_ref<T>::compare(
  const buffer_ref other
  ) const
{
  return memcmp(_begin, other._begin, get_size());
}

template <
  typename T
>
void
buffer_ref<T>::copy_to(
  mutable_buffer_ref<T> other,
  size_type size
  ) const
{
  size = size == (size_type)-1
    ? min(get_size(), other.get_size())
    : size;

  memcpy(other._begin, _begin, size);
}

template <
  typename T
>
buffer_ref<T>
buffer_ref<T>::slice(
  size_type begin,
  size_type end
  ) const
{
  end = end == (size_type)-1
    ? get_size()
    : end;

  return buffer_ref(_begin + begin, _begin + end);
}

//
// non-member operations.
//

template <
  typename T
>
bool
operator==(
  const buffer_ref<T>& lhs,
  const buffer_ref<T>& rhs
  )
{
  return lhs.equals(rhs);
}

template <
  typename T
>
void
swap(
  buffer_ref<T>& lhs,
  buffer_ref<T>& rhs
  )
{
  lhs.swap(rhs);
}

//
// constructors.
//

template <
  typename T
>
mutable_buffer_ref<T>::mutable_buffer_ref(
  void
  )
  : buffer_ref<T>()
{

}

template <
  typename T
>
mutable_buffer_ref<T>::mutable_buffer_ref(
  const mutable_buffer_ref& other
  )
  : buffer_ref<T>(other)
{

}

template <
  typename T
>
mutable_buffer_ref<T>::mutable_buffer_ref(
  mutable_buffer_ref&& other
  )
  : buffer_ref<T>(std::move(other))
{

}

template <
  typename T
>
template <
  typename ITERATOR_TYPE
>
mutable_buffer_ref<T>::mutable_buffer_ref(
  ITERATOR_TYPE begin,
  ITERATOR_TYPE end
  )
{
  _begin = (T*)begin;
  _end = (T*)end;
}

template <
  typename T
>
template <
  typename U,
  size_type N
>
mutable_buffer_ref<T>::mutable_buffer_ref(
  U (&array)[N]
  )
{
  _begin = (T*)array;
  _end = (T*)array + N;
}

//
// destructor.
//

template <
  typename T
>
mutable_buffer_ref<T>::~mutable_buffer_ref(
  void
  )
{

}

//
// assign operators.
//

template <
  typename T
>
mutable_buffer_ref<T>&
mutable_buffer_ref<T>::operator=(
  const mutable_buffer_ref& other
  )
{
  buffer_ref::operator=(other);

  return *this;
}

template <
  typename T
>
mutable_buffer_ref<T>&
mutable_buffer_ref<T>::operator=(
  mutable_buffer_ref&& other
  )
{
  buffer_ref::operator=(std::move(other));

  return *this;
}

//
// swap.
//

template <
  typename T
>
void
mutable_buffer_ref<T>::swap(
  mutable_buffer_ref& other
  )
{
  buffer_ref::swap(other);
}

//
// element access.
//

template <
  typename T
>
typename mutable_buffer_ref<T>::reference
mutable_buffer_ref<T>::operator[](
  size_type index
  )
{
  return at(index);
}

template <
  typename T
>
typename mutable_buffer_ref<T>::reference
mutable_buffer_ref<T>::at(
  size_type index
  )
{
  return _begin[index];
}

template <
  typename T
>
typename buffer_ref<T>::value_type*
mutable_buffer_ref<T>::get_buffer(
  void
  )
{
  return _begin;
}

//
// iterators.
//

template <
  typename T
>
typename mutable_buffer_ref<T>::iterator
mutable_buffer_ref<T>::begin(
  void
  )
{
  return _begin;
}

template <
  typename T
>
typename mutable_buffer_ref<T>::iterator
mutable_buffer_ref<T>::end(
  void
  )
{
  return _end;
}

//
// pointer arithmetic.
//

template <
  typename T
>
typename mutable_buffer_ref<T>::pointer
mutable_buffer_ref<T>::operator++(
  void
  )
{
  return ++_begin;
}

template <
  typename T
>
typename mutable_buffer_ref<T>::pointer
mutable_buffer_ref<T>::operator++(
  int
  )
{
  return _begin++;
}

template <
  typename T
>
typename mutable_buffer_ref<T>::pointer
mutable_buffer_ref<T>::operator--(
  void
  )
{
  return --_begin;
}

template <
  typename T
>
typename mutable_buffer_ref<T>::pointer
mutable_buffer_ref<T>::operator--(
  int
  )
{
  return _begin--;
}


//
// operations.
//

template <
  typename T
>
mutable_buffer_ref<T>&
mutable_buffer_ref<T>::zero_buffer(
  void
  )
{
  memset(_begin, 0, get_size());
  return *this;
}

template <
  typename T
>
mutable_buffer_ref<T>&
mutable_buffer_ref<T>::copy_from(
  const buffer_ref other
  )
{
  size_type size_to_copy = min(get_size(), other.get_size());
  memcpy(_begin, other._begin, size_to_copy);
  return *this;
}

template <
  typename T
>
mutable_buffer_ref<T>
mutable_buffer_ref<T>::slice(
  size_type begin,
  size_type end
  )
{
  end = end == (size_type)-1
    ? _end
    : end;

  return buffer_ref(_begin + begin, _begin + end);
}

}
