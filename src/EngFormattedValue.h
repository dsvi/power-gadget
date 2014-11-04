#ifndef ENGFORMATTEDVALUE_H
#define ENGFORMATTEDVALUE_H

/// "Engineeringly" formats values with prefixes kilo, mega, and so on
class EngFormattedValue
{
public:
	/** loads language dependent prefixes for the f()
	 *  like "y z a f p n µ m _ k M G T P E Z Y"
	 *  as most statics, this one is not thread safe ;) make sure no one calls other methods at the same time
	 */
	static void loadPrefixes();

	/**
	 * Returns string, representing engineeringly formatted value v, with added perfix kilo, mega, and so on.
	 * Example:
	 *	f(12345, 3) -> "12.3k"
	 *	f(123456, 2) -> "0.12M" note leading 0 is not counted towarsds numSignificantDigits.
	 *	f(1.23456, 3) -> "1.23"
	 *	f(1, 3) -> "1.00"
	 *	f(0, 3) -> "0"
	 * dont forget to call loadPrefixes() before using this
	 * @param v the value to format. should be positive
	 * @param numSignificantDigits amount of significant figures to show. must be >= 2
	 */
	static QString f(double v, int numSignificantDigits);
};

#endif // f_H
