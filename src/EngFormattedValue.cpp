#include "EngFormattedValue.h"

//private final static float prefixStartValue = 1e12f; // это должно отражать максимальное значение префикса в массиве ниже
//private final static String[] valuePrefixes = {"Т","Г","М","к","","м","мк"};
static double prefixStartValue = 1e24; // this must reflect the maximum value of the prefix in the array below
static std::vector<std::string> valuePrefixes;

void EngFormattedValue::loadPrefixes(){
	QString prefs = QApplication::translate("SI system prefixes",
		"Y Z E P T G M k _ m µ n p f a z y" );
	valuePrefixes.clear();
	QString s;
	for (auto p : prefs){
		if (p != ' '){
			s += p;
		}
		else {
			if (s == "_")
				s = "";
			valuePrefixes.push_back(s.toUtf8().data());
			s.clear();
		}
	}
	valuePrefixes.push_back(s.toUtf8().data());
}

QString EngFormattedValue::f(double v, int numDigits){
	ASSERT(numDigits >= 2);
	ASSERT(v >= 0);
	ASSERT(valuePrefixes.size() > 0); // forgot loadPrefixes()?
	if (v == 0)
		return "0";
	double floor = prefixStartValue;
	for ( size_t i = 0; i < valuePrefixes.size(); i++ ){
		if ( floor <= v || i == valuePrefixes.size() -1 ){
			float printedValue = v/floor;
			int numIntDigits = 0;
			for (int tens = 1; printedValue > tens; tens *= 10 ){
				numIntDigits++;
			}
			int numFractionalDigits = numDigits - numIntDigits;
			// commented below is for the case, when numDigits is less than 3
			if (numFractionalDigits < 0){
				if (i == 0){
					numFractionalDigits = 0;
				}
				else{
					--i;
					printedValue = v/(floor * 1000);
					numFractionalDigits = numDigits;
				}
			}
//			QString ret;
//			QTextStream fmt(&ret);
//			fmt << qSetRealNumberPrecision(numFractionalDigits) << printedValue << valuePrefixes[i] << flush;
//			return ret;
			QString format = QString().sprintf("%%.%df%%s", numFractionalDigits);
			return QString().sprintf(format.toLatin1(), printedValue, valuePrefixes[i].c_str());
		}
		floor /= 1000;
	}
	ASSERT(false); //wtf?
	return "???";
}


