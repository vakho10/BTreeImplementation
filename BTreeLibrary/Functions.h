#pragma once

/*
	EXTRACTORS
*/
// Basic key extractor (returns plain object)
template<typename T>
struct PlaintKeyExtractor : std::unary_function<T, T>
{
	result_type operator()(argument_type& elem)
	{
		return elem;
	}
};

/*
	COMPARATORS

	You should pass key extractor when you are
	dealling with objects and not primitives.
*/
// Less comparator
template<typename T, typename Extract = PlaintKeyExtractor<T>>
struct Less : std::binary_function<T, T, bool>
{
	Extract ext;
	Less(Extract& _ext = Extract()) 
	{
		ext = _ext;
	}

	result_type operator()(first_argument_type& firstArg, second_argument_type& secondArg)
	{
		return ext(firstArg) < ext(secondArg);
	}
};