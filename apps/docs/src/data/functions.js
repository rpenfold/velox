export const functionCategories = {
  all: {
    name: 'All Functions',
    description: 'All available functions across all categories',
    functions: {} // Will be populated dynamically
  },
  math: {
    name: 'Math',
    description: 'Mathematical and statistical functions',
    functions: {
      SUM: {
        name: 'SUM',
        description: 'Adds all the numbers in a range of cells',
        syntax: 'SUM(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'The first number to add', required: true },
          { name: 'number2', description: 'Additional numbers to add', required: false }
        ],
        examples: [
          { formula: 'SUM(1, 2, 3)', result: '6', description: 'Simple addition' },
          { formula: 'SUM(A1, B1, C1)', result: '15', description: 'Sum of cells (A1=5, B1=4, C1=6)' }
        ]
      },
      SUMX2MY2: {
        name: 'SUMX2MY2',
        description: 'Sum of the difference of squares of corresponding values: Σ(x^2 − y^2)',
        syntax: 'SUMX2MY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMX2MY2([1,2],[3,4])', result: '-20', description: '(1^2-3^2)+(2^2-4^2)= -20' }
        ]
      },
      SUMX2PY2: {
        name: 'SUMX2PY2',
        description: 'Sum of the sum of squares of corresponding values: Σ(x^2 + y^2)',
        syntax: 'SUMX2PY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMX2PY2([1,2],[3,4])', result: '30', description: '(1^2+3^2)+(2^2+4^2)=30' }
        ]
      },
      SUMXMY2: {
        name: 'SUMXMY2',
        description: 'Sum of squares of differences of corresponding values: Σ(x − y)^2',
        syntax: 'SUMXMY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMXMY2([5,7],[2,4])', result: '18', description: '(5-2)^2 + (7-4)^2 = 18' }
        ]
      },
      MAX: {
        name: 'MAX',
        description: 'Returns the largest value among the arguments',
        syntax: 'MAX(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'The first number to compare', required: true },
          { name: 'number2', description: 'Additional numbers to compare', required: false }
        ],
        examples: [
          { formula: 'MAX(10, 20, 5)', result: '20', description: 'Maximum of three numbers' },
          { formula: 'MAX(A1:A5)', result: '25', description: 'Maximum in range A1:A5' }
        ]
      },
      MIN: {
        name: 'MIN',
        description: 'Returns the smallest value among the arguments',
        syntax: 'MIN(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'The first number to compare', required: true },
          { name: 'number2', description: 'Additional numbers to compare', required: false }
        ],
        examples: [
          { formula: 'MIN(10, 20, 5)', result: '5', description: 'Minimum of three numbers' },
          { formula: 'MIN(A1:A5)', result: '2', description: 'Minimum in range A1:A5' }
        ]
      },
      AVERAGE: {
        name: 'AVERAGE',
        description: 'Returns the average (arithmetic mean) of the arguments',
        syntax: 'AVERAGE(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'The first number to average', required: true },
          { name: 'number2', description: 'Additional numbers to average', required: false }
        ],
        examples: [
          { formula: 'AVERAGE(1, 2, 3, 4, 5)', result: '3', description: 'Average of five numbers' },
          { formula: 'AVERAGE(A1:A10)', result: '7.5', description: 'Average of range A1:A10' }
        ]
      },
      ROUND: {
        name: 'ROUND',
        description: 'Rounds a number to a specified number of digits',
        syntax: 'ROUND(number, num_digits)',
        parameters: [
          { name: 'number', description: 'The number to round', required: true },
          { name: 'num_digits', description: 'The number of digits to round to', required: true }
        ],
        examples: [
          { formula: 'ROUND(3.14159, 2)', result: '3.14', description: 'Round to 2 decimal places' },
          { formula: 'ROUND(1234.5, -2)', result: '1200', description: 'Round to nearest hundred' }
        ]
      },
      ABS: {
        name: 'ABS',
        description: 'Returns the absolute value of a number',
        syntax: 'ABS(number)',
        parameters: [
          { name: 'number', description: 'The number to get absolute value of', required: true }
        ],
        examples: [
          { formula: 'ABS(-5)', result: '5', description: 'Absolute value of negative number' },
          { formula: 'ABS(3.14)', result: '3.14', description: 'Absolute value of positive number' }
        ]
      },
      SQRT: {
        name: 'SQRT',
        description: 'Returns the square root of a number',
        syntax: 'SQRT(number)',
        parameters: [
          { name: 'number', description: 'The number to get square root of', required: true }
        ],
        examples: [
          { formula: 'SQRT(16)', result: '4', description: 'Square root of 16' },
          { formula: 'SQRT(2)', result: '1.414', description: 'Square root of 2' }
        ]
      },
      POWER: {
        name: 'POWER',
        description: 'Returns a number raised to a power',
        syntax: 'POWER(number, power)',
        parameters: [
          { name: 'number', description: 'The base number', required: true },
          { name: 'power', description: 'The exponent', required: true }
        ],
        examples: [
          { formula: 'POWER(2, 3)', result: '8', description: '2 to the power of 3' },
          { formula: 'POWER(9, 0.5)', result: '3', description: 'Square root using power' }
        ]
      },
      PI: {
        name: 'PI',
        description: 'Returns the value of pi (π)',
        syntax: 'PI()',
        parameters: [],
        examples: [
          { formula: 'PI()', result: '3.14159', description: 'Value of pi' },
          { formula: 'PI() * 2', result: '6.28318', description: 'Two times pi' }
        ]
      },
      SIN: {
        name: 'SIN',
        description: 'Returns the sine of an angle (in radians)',
        syntax: 'SIN(angle)',
        parameters: [
          { name: 'angle', description: 'The angle in radians', required: true }
        ],
        examples: [
          { formula: 'SIN(PI()/2)', result: '1', description: 'Sine of 90 degrees (π/2 radians)' },
          { formula: 'SIN(0)', result: '0', description: 'Sine of 0 radians' }
        ]
      },
      COS: {
        name: 'COS',
        description: 'Returns the cosine of an angle (in radians)',
        syntax: 'COS(angle)',
        parameters: [
          { name: 'angle', description: 'The angle in radians', required: true }
        ],
        examples: [
          { formula: 'COS(0)', result: '1', description: 'Cosine of 0 radians' },
          { formula: 'COS(PI())', result: '-1', description: 'Cosine of π radians' }
        ]
      },
      TAN: {
        name: 'TAN',
        description: 'Returns the tangent of an angle (in radians)',
        syntax: 'TAN(angle)',
        parameters: [
          { name: 'angle', description: 'The angle in radians', required: true }
        ],
        examples: [
          { formula: 'TAN(PI()/4)', result: '1', description: 'Tangent of 45 degrees (π/4 radians)' },
          { formula: 'TAN(0)', result: '0', description: 'Tangent of 0 radians' }
        ]
      },
      LN: {
        name: 'LN',
        description: 'Returns the natural logarithm of a number',
        syntax: 'LN(number)',
        parameters: [
          { name: 'number', description: 'The number to get natural log of', required: true }
        ],
        examples: [
          { formula: 'LN(EXP(1))', result: '1', description: 'Natural log of e' },
          { formula: 'LN(10)', result: '2.303', description: 'Natural log of 10' }
        ]
      },
      LOG: {
        name: 'LOG',
        description: 'Returns the logarithm of a number to a specified base',
        syntax: 'LOG(number, [base])',
        parameters: [
          { name: 'number', description: 'The number to get logarithm of', required: true },
          { name: 'base', description: 'The base (default: 10)', required: false }
        ],
        examples: [
          { formula: 'LOG(100)', result: '2', description: 'Base 10 log of 100' },
          { formula: 'LOG(8, 2)', result: '3', description: 'Base 2 log of 8' }
        ]
      },
      EXP: {
        name: 'EXP',
        description: 'Returns e raised to the power of a number',
        syntax: 'EXP(number)',
        parameters: [
          { name: 'number', description: 'The exponent', required: true }
        ],
        examples: [
          { formula: 'EXP(1)', result: '2.718', description: 'e to the power of 1' },
          { formula: 'EXP(0)', result: '1', description: 'e to the power of 0' }
        ]
      },
      FACT: {
        name: 'FACT',
        description: 'Returns the factorial of a number',
        syntax: 'FACT(number)',
        parameters: [
          { name: 'number', description: 'The number to get factorial of', required: true }
        ],
        examples: [
          { formula: 'FACT(5)', result: '120', description: 'Factorial of 5 (5! = 5×4×3×2×1)' },
          { formula: 'FACT(0)', result: '1', description: 'Factorial of 0' }
        ]
      },
      CEILING: {
        name: 'CEILING',
        description: 'Rounds a number up to the nearest multiple of significance',
        syntax: 'CEILING(number, [significance])',
        parameters: [
          { name: 'number', description: 'The number to round up', required: true },
          { name: 'significance', description: 'The multiple to round up to', required: false }
        ],
        examples: [
          { formula: 'CEILING(4.3)', result: '5', description: 'Round up to nearest integer' },
          { formula: 'CEILING(4.3, 0.5)', result: '4.5', description: 'Round up to nearest 0.5' }
        ]
      },
      FLOOR: {
        name: 'FLOOR',
        description: 'Rounds a number down to the nearest multiple of significance',
        syntax: 'FLOOR(number, [significance])',
        parameters: [
          { name: 'number', description: 'The number to round down', required: true },
          { name: 'significance', description: 'The multiple to round down to', required: false }
        ],
        examples: [
          { formula: 'FLOOR(4.7)', result: '4', description: 'Round down to nearest integer' },
          { formula: 'FLOOR(4.7, 0.5)', result: '4.5', description: 'Round down to nearest 0.5' }
        ]
      },
      MOD: {
        name: 'MOD',
        description: 'Returns the remainder after division',
        syntax: 'MOD(number, divisor)',
        parameters: [
          { name: 'number', description: 'The number to divide', required: true },
          { name: 'divisor', description: 'The divisor', required: true }
        ],
        examples: [
          { formula: 'MOD(10, 3)', result: '1', description: 'Remainder of 10 divided by 3' },
          { formula: 'MOD(15, 4)', result: '3', description: 'Remainder of 15 divided by 4' }
        ]
      },
      GCD: {
        name: 'GCD',
        description: 'Returns the greatest common divisor of two or more numbers',
        syntax: 'GCD(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'GCD(12, 18)', result: '6', description: 'GCD of 12 and 18' },
          { formula: 'GCD(24, 36, 48)', result: '12', description: 'GCD of multiple numbers' }
        ]
      },
      LCM: {
        name: 'LCM',
        description: 'Returns the least common multiple of two or more numbers',
        syntax: 'LCM(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'LCM(4, 6)', result: '12', description: 'LCM of 4 and 6' },
          { formula: 'LCM(3, 4, 5)', result: '60', description: 'LCM of multiple numbers' }
        ]
      },
      COUNT: {
        name: 'COUNT',
        description: 'Counts the number of numeric values in a range',
        syntax: 'COUNT(value1, [value2], ...)',
        parameters: [
          { name: 'value1', description: 'First value or range to count', required: true },
          { name: 'value2', description: 'Additional values to count', required: false }
        ],
        examples: [
          { formula: 'COUNT(1, 2, 3, "text")', result: '3', description: 'Count numeric values only' },
          { formula: 'COUNT(A1:A10)', result: '8', description: 'Count numbers in range' }
        ]
      },
      MEDIAN: {
        name: 'MEDIAN',
        description: 'Returns the median (middle value) of a set of numbers',
        syntax: 'MEDIAN(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number or range', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'MEDIAN(1, 2, 3, 4, 5)', result: '3', description: 'Median of odd count' },
          { formula: 'MEDIAN(1, 2, 3, 4)', result: '2.5', description: 'Median of even count' }
        ]
      },
      MODE: {
        name: 'MODE',
        description: 'Returns the most frequently occurring value in a set',
        syntax: 'MODE(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number or range', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'MODE(1, 2, 2, 3, 4)', result: '2', description: 'Most frequent value' },
          { formula: 'MODE(A1:A10)', result: '5', description: 'Mode of range' }
        ]
      },
      STDEV: {
        name: 'STDEV',
        description: 'Returns the standard deviation of a sample',
        syntax: 'STDEV(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number or range', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'STDEV(1, 2, 3, 4, 5)', result: '1.58', description: 'Standard deviation of sample' },
          { formula: 'STDEV(A1:A10)', result: '2.87', description: 'Standard deviation of range' }
        ]
      },
      VAR: {
        name: 'VAR',
        description: 'Returns the variance of a sample',
        syntax: 'VAR(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'First number or range', required: true },
          { name: 'number2', description: 'Additional numbers', required: false }
        ],
        examples: [
          { formula: 'VAR(1, 2, 3, 4, 5)', result: '2.5', description: 'Variance of sample' },
          { formula: 'VAR(A1:A10)', result: '8.22', description: 'Variance of range' }
        ]
      },
      RAND: {
        name: 'RAND',
        description: 'Returns a random number between 0 and 1',
        syntax: 'RAND()',
        parameters: [],
        examples: [
          { formula: 'RAND()', result: '0.7234', description: 'Random decimal between 0 and 1' },
          { formula: 'RAND() * 100', result: '72.34', description: 'Random number between 0 and 100' }
        ]
      },
      RANDBETWEEN: {
        name: 'RANDBETWEEN',
        description: 'Returns a random integer between two specified numbers',
        syntax: 'RANDBETWEEN(bottom, top)',
        parameters: [
          { name: 'bottom', description: 'The lowest integer to return', required: true },
          { name: 'top', description: 'The highest integer to return', required: true }
        ],
        examples: [
          { formula: 'RANDBETWEEN(1, 10)', result: '7', description: 'Random integer between 1 and 10' },
          { formula: 'RANDBETWEEN(-5, 5)', result: '-2', description: 'Random integer between -5 and 5' }
        ]
      },
      SUMSQ: {
        name: 'SUMSQ',
        description: 'Returns the sum of the squares of the arguments',
        syntax: 'SUMSQ(number1, [number2], ...)',
        parameters: [
          { name: 'number1', description: 'The first number to square and sum', required: true },
          { name: 'number2', description: 'Additional numbers to square and sum', required: false }
        ],
        examples: [
          { formula: 'SUMSQ(2, 3, 4)', result: '29', description: '2² + 3² + 4² = 4 + 9 + 16 = 29' },
          { formula: 'SUMSQ(1, -2, 3)', result: '14', description: '1² + (-2)² + 3² = 1 + 4 + 9 = 14' }
        ]
      },
      QUOTIENT: {
        name: 'QUOTIENT',
        description: 'Returns the integer portion of a division',
        syntax: 'QUOTIENT(numerator, denominator)',
        parameters: [
          { name: 'numerator', description: 'The dividend', required: true },
          { name: 'denominator', description: 'The divisor', required: true }
        ],
        examples: [
          { formula: 'QUOTIENT(10, 3)', result: '3', description: '10 ÷ 3 = 3.33..., truncated to 3' },
          { formula: 'QUOTIENT(-10, 3)', result: '-3', description: 'Truncates toward zero' }
        ]
      },
      EVEN: {
        name: 'EVEN',
        description: 'Rounds a number up to the nearest even integer',
        syntax: 'EVEN(number)',
        parameters: [
          { name: 'number', description: 'The value to round to the nearest even integer', required: true }
        ],
        examples: [
          { formula: 'EVEN(1.5)', result: '2', description: 'Rounds up to next even integer' },
          { formula: 'EVEN(-1)', result: '-2', description: 'Rounds away from zero to even' }
        ]
      },
      ODD: {
        name: 'ODD',
        description: 'Rounds a number up to the nearest odd integer',
        syntax: 'ODD(number)',
        parameters: [
          { name: 'number', description: 'The value to round to the nearest odd integer', required: true }
        ],
        examples: [
          { formula: 'ODD(2)', result: '3', description: 'Rounds up to next odd integer' },
          { formula: 'ODD(0)', result: '1', description: 'Zero rounds to 1' }
        ]
      },
      MROUND: {
        name: 'MROUND',
        description: 'Rounds a number to the nearest multiple',
        syntax: 'MROUND(number, multiple)',
        parameters: [
          { name: 'number', description: 'The value to round', required: true },
          { name: 'multiple', description: 'The multiple to which to round', required: true }
        ],
        examples: [
          { formula: 'MROUND(10, 3)', result: '9', description: 'Nearest multiple of 3 to 10' },
          { formula: 'MROUND(1.3, 0.2)', result: '1.4', description: 'Nearest multiple of 0.2 to 1.3' }
        ]
      },
      ROUNDUP: {
        name: 'ROUNDUP',
        description: 'Rounds a number up, away from zero',
        syntax: 'ROUNDUP(number, num_digits)',
        parameters: [
          { name: 'number', description: 'The number to round up', required: true },
          { name: 'num_digits', description: 'The number of digits to which to round', required: true }
        ],
        examples: [
          { formula: 'ROUNDUP(3.2, 0)', result: '4', description: 'Always rounds away from zero' },
          { formula: 'ROUNDUP(-3.2, 0)', result: '-4', description: 'Away from zero means more negative' }
        ]
      },
      ROUNDDOWN: {
        name: 'ROUNDDOWN',
        description: 'Rounds a number down, toward zero',
        syntax: 'ROUNDDOWN(number, num_digits)',
        parameters: [
          { name: 'number', description: 'The number to round down', required: true },
          { name: 'num_digits', description: 'The number of digits to which to round', required: true }
        ],
        examples: [
          { formula: 'ROUNDDOWN(3.9, 0)', result: '3', description: 'Always rounds toward zero' },
          { formula: 'ROUNDDOWN(-3.9, 0)', result: '-3', description: 'Toward zero means less negative' }
        ]
      },
      CORREL: {
        name: 'CORREL',
        description: 'Returns the correlation coefficient between two data sets',
        syntax: 'CORREL(array1, array2)',
        parameters: [
          { name: 'array1', description: 'First range or array of numbers', required: true },
          { name: 'array2', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'CORREL([1,2,3,4],[2,4,6,8])', result: '1', description: 'Perfect positive correlation' },
          { formula: 'CORREL([1,2,3],[3,2,1])', result: '-1', description: 'Perfect negative correlation' }
        ]
      },
      SUMX2MY2: {
        name: 'SUMX2MY2',
        description: 'Sum of the difference of squares of corresponding values: Σ(x^2 − y^2)',
        syntax: 'SUMX2MY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMX2MY2([1,2],[3,4])', result: '-20', description: '(1^2-3^2) + (2^2-4^2) = -20' }
        ]
      },
      SUMX2PY2: {
        name: 'SUMX2PY2',
        description: 'Sum of the sum of squares of corresponding values: Σ(x^2 + y^2)',
        syntax: 'SUMX2PY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMX2PY2([1,2],[3,4])', result: '30', description: '(1^2+3^2) + (2^2+4^2) = 30' }
        ]
      },
      SUMXMY2: {
        name: 'SUMXMY2',
        description: 'Sum of squares of differences of corresponding values: Σ(x − y)^2',
        syntax: 'SUMXMY2(array_x, array_y)',
        parameters: [
          { name: 'array_x', description: 'First range or array of numbers', required: true },
          { name: 'array_y', description: 'Second range or array of numbers', required: true }
        ],
        examples: [
          { formula: 'SUMXMY2([5,7],[2,4])', result: '18', description: '(5-2)^2 + (7-4)^2 = 18' }
        ]
      }
    }
  },
  text: {
    name: 'Text',
    description: 'Text manipulation and formatting functions',
    functions: {
      CONCATENATE: {
        name: 'CONCATENATE',
        description: 'Joins two or more text strings into one string',
        syntax: 'CONCATENATE(text1, [text2], ...)',
        parameters: [
          { name: 'text1', description: 'The first text string', required: true },
          { name: 'text2', description: 'Additional text strings', required: false }
        ],
        examples: [
          { formula: 'CONCATENATE("Hello", " ", "World")', result: '"Hello World"', description: 'Join three strings' },
          { formula: 'CONCATENATE(A1, " - ", B1)', result: '"Name - Value"', description: 'Join cell values with separator' }
        ]
      },
      LEN: {
        name: 'LEN',
        description: 'Returns the number of characters in a text string',
        syntax: 'LEN(text)',
        parameters: [
          { name: 'text', description: 'The text string to measure', required: true }
        ],
        examples: [
          { formula: 'LEN("Hello")', result: '5', description: 'Length of text string' },
          { formula: 'LEN(A1)', result: '12', description: 'Length of text in cell A1' }
        ]
      },
      UPPER: {
        name: 'UPPER',
        description: 'Converts text to uppercase',
        syntax: 'UPPER(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to uppercase', required: true }
        ],
        examples: [
          { formula: 'UPPER("hello world")', result: '"HELLO WORLD"', description: 'Convert to uppercase' },
          { formula: 'UPPER(A1)', result: '"EXCEL FORMULA"', description: 'Convert cell value to uppercase' }
        ]
      },
      LOWER: {
        name: 'LOWER',
        description: 'Converts text to lowercase',
        syntax: 'LOWER(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to lowercase', required: true }
        ],
        examples: [
          { formula: 'LOWER("HELLO WORLD")', result: '"hello world"', description: 'Convert to lowercase' },
          { formula: 'LOWER(A1)', result: '"excel formula"', description: 'Convert cell value to lowercase' }
        ]
      }
    }
  },
  logical: {
    name: 'Logical',
    description: 'Logical and conditional functions',
    functions: {
      IF: {
        name: 'IF',
        description: 'Returns one value if condition is TRUE, another if FALSE',
        syntax: 'IF(logical_test, value_if_true, value_if_false)',
        parameters: [
          { name: 'logical_test', description: 'The condition to test', required: true },
          { name: 'value_if_true', description: 'Value to return if condition is TRUE', required: true },
          { name: 'value_if_false', description: 'Value to return if condition is FALSE', required: true }
        ],
        examples: [
          { formula: 'IF(5 > 3, "Yes", "No")', result: '"Yes"', description: 'Simple condition test' },
          { formula: 'IF(A1 > 100, "High", "Low")', result: '"High"', description: 'Compare cell value' }
        ]
      },
      AND: {
        name: 'AND',
        description: 'Returns TRUE if all arguments are TRUE',
        syntax: 'AND(logical1, [logical2], ...)',
        parameters: [
          { name: 'logical1', description: 'The first logical value', required: true },
          { name: 'logical2', description: 'Additional logical values', required: false }
        ],
        examples: [
          { formula: 'AND(5 > 3, 10 < 20)', result: 'TRUE', description: 'Both conditions are true' },
          { formula: 'AND(A1 > 0, B1 < 100)', result: 'FALSE', description: 'At least one condition is false' }
        ]
      },
      OR: {
        name: 'OR',
        description: 'Returns TRUE if any argument is TRUE',
        syntax: 'OR(logical1, [logical2], ...)',
        parameters: [
          { name: 'logical1', description: 'The first logical value', required: true },
          { name: 'logical2', description: 'Additional logical values', required: false }
        ],
        examples: [
          { formula: 'OR(5 > 10, 3 < 5)', result: 'TRUE', description: 'One condition is true' },
          { formula: 'OR(A1 < 0, B1 > 100)', result: 'FALSE', description: 'Both conditions are false' }
        ]
      },
      SWITCH: {
        name: 'SWITCH',
        description: 'Compares an expression against multiple values and returns corresponding result',
        syntax: 'SWITCH(expression, value1, result1, [value2, result2, ...], [default])',
        parameters: [
          { name: 'expression', description: 'The value to compare against', required: true },
          { name: 'value1', description: 'First value to compare with expression', required: true },
          { name: 'result1', description: 'Result to return if value1 matches', required: true },
          { name: 'value2', description: 'Additional values to compare (optional)', required: false },
          { name: 'result2', description: 'Additional results (optional)', required: false },
          { name: 'default', description: 'Default value if no match found (optional)', required: false }
        ],
        examples: [
          { formula: 'SWITCH(2, 1, "One", 2, "Two", 3, "Three")', result: '"Two"', description: 'Match found for value 2' },
          { formula: 'SWITCH(4, 1, "One", 2, "Two", "Default")', result: '"Default"', description: 'No match, return default' }
        ]
      },
      IFS: {
        name: 'IFS',
        description: 'Checks multiple conditions and returns result for first TRUE condition',
        syntax: 'IFS(condition1, result1, [condition2, result2, ...])',
        parameters: [
          { name: 'condition1', description: 'First logical condition to test', required: true },
          { name: 'result1', description: 'Result to return if condition1 is TRUE', required: true },
          { name: 'condition2', description: 'Additional conditions to test (optional)', required: false },
          { name: 'result2', description: 'Additional results (optional)', required: false }
        ],
        examples: [
          { formula: 'IFS(FALSE, "First", TRUE, "Second")', result: '"Second"', description: 'First TRUE condition wins' },
          { formula: 'IFS(0, "Zero", 1, "One")', result: '"One"', description: 'Number conditions: 0=FALSE, 1=TRUE' }
        ]
      }
    }
  },
  text: {
    name: 'Text',
    description: 'Text manipulation and formatting functions',
    functions: {
      CONCATENATE: {
        name: 'CONCATENATE',
        description: 'Joins several text strings into one text string',
        syntax: 'CONCATENATE(text1, [text2], ...)',
        parameters: [
          { name: 'text1', description: 'The first text string to join', required: true },
          { name: 'text2', description: 'Additional text strings to join', required: false }
        ],
        examples: [
          { formula: 'CONCATENATE("Hello", " ", "World")', result: '"Hello World"', description: 'Join text strings' },
          { formula: 'CONCATENATE(A1, " - ", B1)', result: '"John - Doe"', description: 'Join cells with separator' }
        ]
      },
      LEFT: {
        name: 'LEFT',
        description: 'Returns the leftmost characters from a text string',
        syntax: 'LEFT(text, [num_chars])',
        parameters: [
          { name: 'text', description: 'The text string from which to extract characters', required: true },
          { name: 'num_chars', description: 'Number of characters to extract (default: 1)', required: false }
        ],
        examples: [
          { formula: 'LEFT("Hello", 2)', result: '"He"', description: 'Get first 2 characters' },
          { formula: 'LEFT(A1)', result: '"H"', description: 'Get first character' }
        ]
      },
      RIGHT: {
        name: 'RIGHT',
        description: 'Returns the rightmost characters from a text string',
        syntax: 'RIGHT(text, [num_chars])',
        parameters: [
          { name: 'text', description: 'The text string from which to extract characters', required: true },
          { name: 'num_chars', description: 'Number of characters to extract (default: 1)', required: false }
        ],
        examples: [
          { formula: 'RIGHT("Hello", 2)', result: '"lo"', description: 'Get last 2 characters' },
          { formula: 'RIGHT(A1)', result: '"o"', description: 'Get last character' }
        ]
      },
      MID: {
        name: 'MID',
        description: 'Returns characters from the middle of a text string',
        syntax: 'MID(text, start_num, num_chars)',
        parameters: [
          { name: 'text', description: 'The text string from which to extract characters', required: true },
          { name: 'start_num', description: 'Position of the first character to extract', required: true },
          { name: 'num_chars', description: 'Number of characters to extract', required: true }
        ],
        examples: [
          { formula: 'MID("Hello World", 7, 5)', result: '"World"', description: 'Extract characters from position 7' },
          { formula: 'MID(A1, 2, 3)', result: '"ell"', description: 'Extract 3 chars starting at position 2' }
        ]
      },
      LEN: {
        name: 'LEN',
        description: 'Returns the number of characters in a text string',
        syntax: 'LEN(text)',
        parameters: [
          { name: 'text', description: 'The text string to measure', required: true }
        ],
        examples: [
          { formula: 'LEN("Hello")', result: '5', description: 'Length of text string' },
          { formula: 'LEN(A1)', result: '10', description: 'Length of cell content' }
        ]
      },
      UPPER: {
        name: 'UPPER',
        description: 'Converts text to uppercase',
        syntax: 'UPPER(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to uppercase', required: true }
        ],
        examples: [
          { formula: 'UPPER("hello")', result: '"HELLO"', description: 'Convert to uppercase' },
          { formula: 'UPPER(A1)', result: '"WORLD"', description: 'Convert cell content to uppercase' }
        ]
      },
      LOWER: {
        name: 'LOWER',
        description: 'Converts text to lowercase',
        syntax: 'LOWER(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to lowercase', required: true }
        ],
        examples: [
          { formula: 'LOWER("HELLO")', result: '"hello"', description: 'Convert to lowercase' },
          { formula: 'LOWER(A1)', result: '"world"', description: 'Convert cell content to lowercase' }
        ]
      },
      RPT: {
        name: 'RPT',
        description: 'Repeats text a specified number of times',
        syntax: 'RPT(text, repeat_count)',
        parameters: [
          { name: 'text', description: 'The text string to repeat', required: true },
          { name: 'repeat_count', description: 'The number of times to repeat the text', required: true }
        ],
        examples: [
          { formula: 'RPT("abc", 3)', result: '"abcabcabc"', description: 'Repeat text three times' },
          { formula: 'RPT("x", 5)', result: '"xxxxx"', description: 'Repeat single character five times' }
        ]
      },
      PROPER: {
        name: 'PROPER',
        description: 'Capitalizes the first letter of each word in text',
        syntax: 'PROPER(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to proper case', required: true }
        ],
        examples: [
          { formula: 'PROPER("hello world")', result: '"Hello World"', description: 'Convert to proper case' },
          { formula: 'PROPER(A1)', result: '"John Doe"', description: 'Convert cell content to proper case' }
        ]
      },
      TRIM: {
        name: 'TRIM',
        description: 'Removes extra spaces from text',
        syntax: 'TRIM(text)',
        parameters: [
          { name: 'text', description: 'The text from which to remove spaces', required: true }
        ],
        examples: [
          { formula: 'TRIM("  Hello  World  ")', result: '"Hello World"', description: 'Remove extra spaces' },
          { formula: 'TRIM(A1)', result: '"Clean Text"', description: 'Clean up cell content' }
        ]
      },
      FIND: {
        name: 'FIND',
        description: 'Finds one text string within another (case-sensitive)',
        syntax: 'FIND(find_text, within_text, [start_num])',
        parameters: [
          { name: 'find_text', description: 'The text to find', required: true },
          { name: 'within_text', description: 'The text in which to search', required: true },
          { name: 'start_num', description: 'Position to start searching from', required: false }
        ],
        examples: [
          { formula: 'FIND("lo", "Hello")', result: '4', description: 'Find position of "lo" in "Hello"' },
          { formula: 'FIND("o", "Hello", 5)', result: '5', description: 'Find "o" starting from position 5' }
        ]
      },
      SEARCH: {
        name: 'SEARCH',
        description: 'Finds one text string within another (case-insensitive)',
        syntax: 'SEARCH(find_text, within_text, [start_num])',
        parameters: [
          { name: 'find_text', description: 'The text to find', required: true },
          { name: 'within_text', description: 'The text in which to search', required: true },
          { name: 'start_num', description: 'Position to start searching from', required: false }
        ],
        examples: [
          { formula: 'SEARCH("LO", "Hello")', result: '4', description: 'Case-insensitive search for "LO"' },
          { formula: 'SEARCH("world", "Hello World")', result: '7', description: 'Find position case-insensitively' }
        ]
      },
      REPLACE: {
        name: 'REPLACE',
        description: 'Replaces characters within text based on position',
        syntax: 'REPLACE(old_text, start_num, num_chars, new_text)',
        parameters: [
          { name: 'old_text', description: 'The original text', required: true },
          { name: 'start_num', description: 'Position to start replacing', required: true },
          { name: 'num_chars', description: 'Number of characters to replace', required: true },
          { name: 'new_text', description: 'The replacement text', required: true }
        ],
        examples: [
          { formula: 'REPLACE("Hello", 2, 2, "i")', result: '"Hilo"', description: 'Replace "el" with "i"' },
          { formula: 'REPLACE(A1, 1, 3, "New")', result: '"NewValue"', description: 'Replace first 3 characters' }
        ]
      },
      SUBSTITUTE: {
        name: 'SUBSTITUTE',
        description: 'Substitutes new text for old text in a string',
        syntax: 'SUBSTITUTE(text, old_text, new_text, [instance_num])',
        parameters: [
          { name: 'text', description: 'The original text', required: true },
          { name: 'old_text', description: 'The text to replace', required: true },
          { name: 'new_text', description: 'The replacement text', required: true },
          { name: 'instance_num', description: 'Which instance to replace (optional)', required: false }
        ],
        examples: [
          { formula: 'SUBSTITUTE("Hello Hello", "Hello", "Hi")', result: '"Hi Hi"', description: 'Replace all instances' },
          { formula: 'SUBSTITUTE("a,b,c", ",", ";")', result: '"a;b;c"', description: 'Replace commas with semicolons' }
        ]
      },
      TEXT: {
        name: 'TEXT',
        description: 'Formats a number as text according to a specified format',
        syntax: 'TEXT(value, format_text)',
        parameters: [
          { name: 'value', description: 'The number to format', required: true },
          { name: 'format_text', description: 'The format to apply', required: true }
        ],
        examples: [
          { formula: 'TEXT(123.45, "0.00")', result: '"123.45"', description: 'Format with 2 decimal places' },
          { formula: 'TEXT(0.75, "0%")', result: '"75%"', description: 'Format as percentage' }
        ]
      },
      CHAR: {
        name: 'CHAR',
        description: 'Returns the character specified by a code number (1-255)',
        syntax: 'CHAR(number)',
        parameters: [
          { name: 'number', description: 'Code number between 1 and 255', required: true }
        ],
        examples: [
          { formula: 'CHAR(65)', result: '"A"', description: 'Character for code 65' },
          { formula: 'CHAR(10)', result: '"\n"', description: 'Line feed character' }
        ]
      },
      CODE: {
        name: 'CODE',
        description: 'Returns a numeric code for the first character in a text string',
        syntax: 'CODE(text)',
        parameters: [
          { name: 'text', description: 'The text to examine', required: true }
        ],
        examples: [
          { formula: 'CODE("A")', result: '65', description: 'Code for letter A' },
          { formula: 'CODE("!Hello")', result: '33', description: 'Code for first character !' }
        ]
      },
      CLEAN: {
        name: 'CLEAN',
        description: 'Removes all nonprintable characters from text (ASCII codes < 32)',
        syntax: 'CLEAN(text)',
        parameters: [
          { name: 'text', description: 'The text from which to remove nonprintable characters', required: true }
        ],
        examples: [
          { formula: 'CLEAN("Hello\nWorld")', result: '"HelloWorld"', description: 'Removes line feed' },
          { formula: 'CLEAN(CHAR(9) & "Tab")', result: '"Tab"', description: 'Removes tab' }
        ]
      },
      EXACT: {
        name: 'EXACT',
        description: 'Checks whether two text strings are exactly the same (case-sensitive)',
        syntax: 'EXACT(text1, text2)',
        parameters: [
          { name: 'text1', description: 'First text string', required: true },
          { name: 'text2', description: 'Second text string', required: true }
        ],
        examples: [
          { formula: 'EXACT("Hello", "Hello")', result: 'TRUE', description: 'Strings match exactly' },
          { formula: 'EXACT("Hello", "hello")', result: 'FALSE', description: 'Case-sensitive comparison' }
        ]
      },
      VALUE: {
        name: 'VALUE',
        description: 'Converts text that represents a number to a number',
        syntax: 'VALUE(text)',
        parameters: [
          { name: 'text', description: 'The text to convert to a number', required: true }
        ],
        examples: [
          { formula: 'VALUE("123")', result: '123', description: 'Convert text to number' },
          { formula: 'VALUE("$1,000.00")', result: '1000', description: 'Convert formatted text to number' }
        ]
      },
      CONCAT: {
        name: 'CONCAT',
        description: 'Joins text strings (alias for CONCATENATE)',
        syntax: 'CONCAT(text1, [text2], ...)',
        parameters: [
          { name: 'text1', description: 'The first text string', required: true },
          { name: 'text2', description: 'Additional text strings', required: false }
        ],
        examples: [
          { formula: 'CONCAT("Hello", " ", "World")', result: '"Hello World"', description: 'Join three strings' },
          { formula: 'CONCAT(A1, B1)', result: '"JohnDoe"', description: 'Join cell values directly' }
        ]
      },
      T: {
        name: 'T',
        description: 'Returns text if the argument is text, otherwise returns empty string',
        syntax: 'T(value)',
        parameters: [
          { name: 'value', description: 'The value to test', required: true }
        ],
        examples: [
          { formula: 'T("Hello")', result: '"Hello"', description: 'Text input returns text' },
          { formula: 'T(123)', result: '""', description: 'Number input returns empty string' }
        ]
      },
      TEXTJOIN: {
        name: 'TEXTJOIN',
        description: 'Joins text values with a delimiter',
        syntax: 'TEXTJOIN(delimiter, ignore_empty, text1, [text2], ...)',
        parameters: [
          { name: 'delimiter', description: 'Text to use as separator', required: true },
          { name: 'ignore_empty', description: 'TRUE to ignore empty values, FALSE to include them', required: true },
          { name: 'text1', description: 'First text value to join', required: true },
          { name: 'text2', description: 'Additional text values to join', required: false }
        ],
        examples: [
          { formula: 'TEXTJOIN(",", TRUE, "A", "", "B", "C")', result: '"A,B,C"', description: 'Join with comma, ignore empty' },
          { formula: 'TEXTJOIN(" ", FALSE, "Hello", "", "World")', result: '"Hello  World"', description: 'Join with space, include empty' }
        ]
      },
      UNICHAR: {
        name: 'UNICHAR',
        description: 'Returns the Unicode character for a given code point',
        syntax: 'UNICHAR(number)',
        parameters: [
          { name: 'number', description: 'Unicode code point (1 to 1114111)', required: true }
        ],
        examples: [
          { formula: 'UNICHAR(65)', result: '"A"', description: 'ASCII code 65 is "A"' },
          { formula: 'UNICHAR(128512)', result: '"😀"', description: 'Unicode emoji character' }
        ]
      },
      UNICODE: {
        name: 'UNICODE',
        description: 'Returns the Unicode code point of the first character in text',
        syntax: 'UNICODE(text)',
        parameters: [
          { name: 'text', description: 'The text containing the character', required: true }
        ],
        examples: [
          { formula: 'UNICODE("A")', result: '65', description: 'Unicode code for "A"' },
          { formula: 'UNICODE("😀")', result: '128512', description: 'Unicode code for emoji' }
        ]
      },
      DEC2OCT: {
        name: 'DEC2OCT',
        description: 'Converts a decimal number to octal',
        syntax: 'DEC2OCT(number, [places])',
        parameters: [
          { name: 'number', description: 'The decimal number to convert', required: true },
          { name: 'places', description: 'Number of characters to use', required: false }
        ],
        examples: [
          { formula: 'DEC2OCT(8)', result: '"10"', description: 'Decimal 8 to octal 10' }
        ]
      },
      BIN2OCT: {
        name: 'BIN2OCT',
        description: 'Converts a binary number to octal',
        syntax: 'BIN2OCT(number)',
        parameters: [
          { name: 'number', description: 'Binary string to convert', required: true }
        ],
        examples: [
          { formula: 'BIN2OCT("1111111111")', result: '"1777"', description: 'Binary to octal' }
        ]
      },
      OCT2BIN: {
        name: 'OCT2BIN',
        description: 'Converts an octal number to binary',
        syntax: 'OCT2BIN(number)',
        parameters: [
          { name: 'number', description: 'Octal string to convert', required: true }
        ],
        examples: [
          { formula: 'OCT2BIN("1777")', result: '"1111111111"', description: 'Octal to binary' }
        ]
      },
      HEX2OCT: {
        name: 'HEX2OCT',
        description: 'Converts a hexadecimal number to octal',
        syntax: 'HEX2OCT(number)',
        parameters: [
          { name: 'number', description: 'Hexadecimal string to convert', required: true }
        ],
        examples: [
          { formula: 'HEX2OCT("FF")', result: '"377"', description: 'Hex to octal' }
        ]
      },
      OCT2HEX: {
        name: 'OCT2HEX',
        description: 'Converts an octal number to hexadecimal',
        syntax: 'OCT2HEX(number)',
        parameters: [
          { name: 'number', description: 'Octal string to convert', required: true }
        ],
        examples: [
          { formula: 'OCT2HEX("377")', result: '"FF"', description: 'Octal to hex' }
        ]
      },
      COMPLEX: {
        name: 'COMPLEX',
        description: 'Converts real and imaginary coefficients into a complex number',
        syntax: 'COMPLEX(real_num, i_num, [suffix])',
        parameters: [
          { name: 'real_num', required: true, description: 'Real coefficient' },
          { name: 'i_num', required: true, description: 'Imaginary coefficient' },
          { name: 'suffix', required: false, description: 'i or j (default i)' }
        ],
        examples: [
          { formula: 'COMPLEX(3, -4)', result: '"3+-4i"', description: 'Standard i suffix' }
        ]
      },
      IMREAL: {
        name: 'IMREAL',
        description: 'Returns the real coefficient of a complex number',
        syntax: 'IMREAL(inumber)',
        parameters: [
          { name: 'inumber', required: true, description: 'Complex number as text' }
        ],
        examples: [
          { formula: 'IMREAL("3-4i")', result: '3', description: 'Extract real part' }
        ]
      },
      IMAGINARY: {
        name: 'IMAGINARY',
        description: 'Returns the imaginary coefficient of a complex number',
        syntax: 'IMAGINARY(inumber)',
        parameters: [
          { name: 'inumber', required: true, description: 'Complex number as text' }
        ],
        examples: [
          { formula: 'IMAGINARY("3-4i")', result: '-4', description: 'Extract imaginary part' }
        ]
      },
      ROMAN: {
        name: 'ROMAN',
        description: 'Converts an Arabic number to Roman numerals',
        syntax: 'ROMAN(number)',
        parameters: [
          { name: 'number', description: 'Positive integer to convert to Roman numerals', required: true }
        ],
        examples: [
          { formula: 'ROMAN(1987)', result: '"MCMLXXXVII"', description: '1987 in Roman numerals' },
          { formula: 'ROMAN(4)', result: '"IV"', description: 'Subtractive notation' }
        ]
      },
      ARABIC: {
        name: 'ARABIC',
        description: 'Converts Roman numerals to an Arabic number',
        syntax: 'ARABIC(text)',
        parameters: [
          { name: 'text', description: 'Roman numeral string to convert', required: true }
        ],
        examples: [
          { formula: 'ARABIC("MCMLXXXVII")', result: '1987', description: 'Roman to Arabic' },
          { formula: 'ARABIC("IV")', result: '4', description: 'Subtractive notation' }
        ]
      }
    }
  },
  financial: {
    name: 'Financial',
    description: 'Financial calculation functions',
    functions: {
      FV: {
        name: 'FV',
        description: 'Returns the future value of an investment',
        syntax: 'FV(rate, nper, pmt, [pv], [type])',
        parameters: [
          { name: 'rate', description: 'Interest rate per period', required: true },
          { name: 'nper', description: 'Number of payment periods', required: true },
          { name: 'pmt', description: 'Payment made each period', required: true },
          { name: 'pv', description: 'Present value (default: 0)', required: false },
          { name: 'type', description: 'Payment timing (0=end, 1=beginning)', required: false }
        ],
        examples: [
          { formula: 'FV(0.05, 10, -100, 0, 0)', result: '1257.79', description: 'Future value of annuity' },
          { formula: 'FV(0.06/12, 24, -200)', result: '5141.85', description: 'Monthly payments future value' }
        ]
      },
      PV: {
        name: 'PV',
        description: 'Returns the present value of an investment',
        syntax: 'PV(rate, nper, pmt, [fv], [type])',
        parameters: [
          { name: 'rate', description: 'Interest rate per period', required: true },
          { name: 'nper', description: 'Number of payment periods', required: true },
          { name: 'pmt', description: 'Payment made each period', required: true },
          { name: 'fv', description: 'Future value (default: 0)', required: false },
          { name: 'type', description: 'Payment timing (0=end, 1=beginning)', required: false }
        ],
        examples: [
          { formula: 'PV(0.08, 10, -100, 0, 0)', result: '671.01', description: 'Present value of annuity' },
          { formula: 'PV(0.05/12, 60, -300)', result: '15761.86', description: 'Present value of loan' }
        ]
      },
      PMT: {
        name: 'PMT',
        description: 'Returns the periodic payment for an annuity',
        syntax: 'PMT(rate, nper, pv, [fv], [type])',
        parameters: [
          { name: 'rate', description: 'Interest rate per period', required: true },
          { name: 'nper', description: 'Number of payment periods', required: true },
          { name: 'pv', description: 'Present value', required: true },
          { name: 'fv', description: 'Future value (default: 0)', required: false },
          { name: 'type', description: 'Payment timing (0=end, 1=beginning)', required: false }
        ],
        examples: [
          { formula: 'PMT(0.05, 10, 1000)', result: '-129.50', description: 'Payment for loan' },
          { formula: 'PMT(0.06/12, 360, 200000)', result: '-1199.10', description: 'Monthly mortgage payment' }
        ]
      },
      NPV: {
        name: 'NPV',
        description: 'Returns the net present value of an investment',
        syntax: 'NPV(rate, value1, [value2], ...)',
        parameters: [
          { name: 'rate', description: 'Discount rate per period', required: true },
          { name: 'value1', description: 'First cash flow value', required: true },
          { name: 'value2', description: 'Additional cash flow values', required: false }
        ],
        examples: [
          { formula: 'NPV(0.1, -1000, 300, 400, 500)', result: '18.78', description: 'NPV of cash flows' },
          { formula: 'NPV(0.08, -5000, 1200, 1500, 2000, 2500)', result: '447.33', description: 'Investment NPV' }
        ]
      },
      IRR: {
        name: 'IRR',
        description: 'Returns the internal rate of return for cash flows',
        syntax: 'IRR(values, [guess])',
        parameters: [
          { name: 'values', description: 'Array of cash flow values', required: true },
          { name: 'guess', description: 'Initial guess for IRR (default: 0.1)', required: false }
        ],
        examples: [
          { formula: 'IRR([-1000, 300, 400, 500])', result: '0.1296', description: 'IRR of cash flows' },
          { formula: 'IRR([-5000, 1200, 1500, 2000, 2500])', result: '0.0899', description: 'Investment IRR' }
        ]
      },
      MIRR: {
        name: 'MIRR',
        description: 'Returns the modified internal rate of return',
        syntax: 'MIRR(values, finance_rate, reinvest_rate)',
        parameters: [
          { name: 'values', description: 'Array of cash flow values', required: true },
          { name: 'finance_rate', description: 'Interest rate for financing costs', required: true },
          { name: 'reinvest_rate', description: 'Interest rate for reinvestment', required: true }
        ],
        examples: [
          { formula: 'MIRR([-1000, 300, 400, 500], 0.1, 0.12)', result: '0.1342', description: 'MIRR with different rates' },
          { formula: 'MIRR([-5000, 1200, 1500, 2000], 0.08, 0.10)', result: '0.0918', description: 'Modified IRR' }
        ]
      },
      NPER: {
        name: 'NPER',
        description: 'Returns the number of periods for an investment',
        syntax: 'NPER(rate, pmt, pv, [fv], [type])',
        parameters: [
          { name: 'rate', description: 'Interest rate per period', required: true },
          { name: 'pmt', description: 'Payment made each period', required: true },
          { name: 'pv', description: 'Present value', required: true },
          { name: 'fv', description: 'Future value (default: 0)', required: false },
          { name: 'type', description: 'Payment timing (0=end, 1=beginning)', required: false }
        ],
        examples: [
          { formula: 'NPER(0.05, -100, 1000)', result: '14.21', description: 'Periods to pay off loan' },
          { formula: 'NPER(0.06/12, -500, 25000)', result: '58.21', description: 'Months to pay off loan' }
        ]
      },
      RATE: {
        name: 'RATE',
        description: 'Returns the interest rate per period of an annuity',
        syntax: 'RATE(nper, pmt, pv, [fv], [type], [guess])',
        parameters: [
          { name: 'nper', description: 'Number of payment periods', required: true },
          { name: 'pmt', description: 'Payment made each period', required: true },
          { name: 'pv', description: 'Present value', required: true },
          { name: 'fv', description: 'Future value (default: 0)', required: false },
          { name: 'type', description: 'Payment timing (0=end, 1=beginning)', required: false },
          { name: 'guess', description: 'Initial guess for rate', required: false }
        ],
        examples: [
          { formula: 'RATE(10, -100, 1000)', result: '0.0507', description: 'Interest rate for loan' },
          { formula: 'RATE(360, -1200, 200000)', result: '0.005', description: 'Monthly mortgage rate' }
        ]
      }
    }
  },
  engineering: {
    name: 'Engineering',
    description: 'Engineering and conversion functions',
    functions: {
      BIN2DEC: {
        name: 'BIN2DEC',
        description: 'Converts a binary number to decimal',
        syntax: 'BIN2DEC(number)',
        parameters: [
          { name: 'number', description: 'The binary number to convert', required: true }
        ],
        examples: [
          { formula: 'BIN2DEC("1010")', result: '10', description: 'Convert binary to decimal' },
          { formula: 'BIN2DEC("11111111")', result: '255', description: 'Convert 8-bit binary' }
        ]
      },
      DEC2BIN: {
        name: 'DEC2BIN',
        description: 'Converts a decimal number to binary',
        syntax: 'DEC2BIN(number, [places])',
        parameters: [
          { name: 'number', description: 'The decimal number to convert', required: true },
          { name: 'places', description: 'Number of characters to use', required: false }
        ],
        examples: [
          { formula: 'DEC2BIN(10)', result: '"1010"', description: 'Convert decimal to binary' },
          { formula: 'DEC2BIN(10, 8)', result: '"00001010"', description: 'Convert with padding' }
        ]
      },
      DEC2HEX: {
        name: 'DEC2HEX',
        description: 'Converts a decimal number to hexadecimal',
        syntax: 'DEC2HEX(number, [places])',
        parameters: [
          { name: 'number', description: 'The decimal number to convert', required: true },
          { name: 'places', description: 'Number of characters to use', required: false }
        ],
        examples: [
          { formula: 'DEC2HEX(255)', result: '"FF"', description: 'Convert decimal to hex' },
          { formula: 'DEC2HEX(10, 4)', result: '"000A"', description: 'Convert with padding' }
        ]
      },
      HEX2DEC: {
        name: 'HEX2DEC',
        description: 'Converts a hexadecimal number to decimal',
        syntax: 'HEX2DEC(number)',
        parameters: [
          { name: 'number', description: 'The hexadecimal number to convert', required: true }
        ],
        examples: [
          { formula: 'HEX2DEC("FF")', result: '255', description: 'Convert hex to decimal' },
          { formula: 'HEX2DEC("A0")', result: '160', description: 'Convert hex with letters' }
        ]
      },
      BITAND: {
        name: 'BITAND',
        description: 'Returns a bitwise AND of two numbers',
        syntax: 'BITAND(number1, number2)',
        parameters: [
          { name: 'number1', description: 'First number for AND operation', required: true },
          { name: 'number2', description: 'Second number for AND operation', required: true }
        ],
        examples: [
          { formula: 'BITAND(5, 3)', result: '1', description: 'Bitwise AND of 5 and 3' },
          { formula: 'BITAND(255, 15)', result: '15', description: 'Mask lower 4 bits' }
        ]
      },
      BITOR: {
        name: 'BITOR',
        description: 'Returns a bitwise OR of two numbers',
        syntax: 'BITOR(number1, number2)',
        parameters: [
          { name: 'number1', description: 'First number for OR operation', required: true },
          { name: 'number2', description: 'Second number for OR operation', required: true }
        ],
        examples: [
          { formula: 'BITOR(5, 3)', result: '7', description: 'Bitwise OR of 5 and 3' },
          { formula: 'BITOR(1, 2)', result: '3', description: 'Combine bit flags' }
        ]
      },
      BITXOR: {
        name: 'BITXOR',
        description: 'Returns a bitwise XOR of two numbers',
        syntax: 'BITXOR(number1, number2)',
        parameters: [
          { name: 'number1', description: 'First number for XOR operation', required: true },
          { name: 'number2', description: 'Second number for XOR operation', required: true }
        ],
        examples: [
          { formula: 'BITXOR(5, 3)', result: '6', description: 'Bitwise XOR of 5 and 3' },
          { formula: 'BITXOR(15, 15)', result: '0', description: 'XOR same numbers' }
        ]
      },
      CONVERT: {
        name: 'CONVERT',
        description: 'Converts a number from one measurement system to another',
        syntax: 'CONVERT(number, from_unit, to_unit)',
        parameters: [
          { name: 'number', description: 'The value to convert', required: true },
          { name: 'from_unit', description: 'The unit to convert from', required: true },
          { name: 'to_unit', description: 'The unit to convert to', required: true }
        ],
        examples: [
          { formula: 'CONVERT(32, "F", "C")', result: '0', description: 'Fahrenheit to Celsius' },
          { formula: 'CONVERT(1, "m", "ft")', result: '3.28084', description: 'Meters to feet' }
        ]
      }
    }
  },
  datetime: {
    name: 'Date & Time',
    description: 'Date and time manipulation functions',
    functions: {
      NOW: {
        name: 'NOW',
        description: 'Returns the current date and time',
        syntax: 'NOW()',
        parameters: [],
        examples: [
          { formula: 'NOW()', result: '2024-01-15 14:30:00', description: 'Current date and time' }
        ]
      },
      TODAY: {
        name: 'TODAY',
        description: 'Returns the current date',
        syntax: 'TODAY()',
        parameters: [],
        examples: [
          { formula: 'TODAY()', result: '2024-01-15', description: 'Current date' }
        ]
      },
      YEAR: {
        name: 'YEAR',
        description: 'Returns the year of a date',
        syntax: 'YEAR(date)',
        parameters: [
          { name: 'date', description: 'The date to extract year from', required: true }
        ],
        examples: [
          { formula: 'YEAR(DATE(2024, 1, 15))', result: '2024', description: 'Extract year from date' },
          { formula: 'YEAR(TODAY())', result: '2024', description: 'Current year' }
        ]
      },
      MONTH: {
        name: 'MONTH',
        description: 'Returns the month of a date',
        syntax: 'MONTH(date)',
        parameters: [
          { name: 'date', description: 'The date to extract month from', required: true }
        ],
        examples: [
          { formula: 'MONTH(DATE(2024, 3, 15))', result: '3', description: 'Extract month from date' },
          { formula: 'MONTH(TODAY())', result: '1', description: 'Current month' }
        ]
      },
      DAY: {
        name: 'DAY',
        description: 'Returns the day of a date',
        syntax: 'DAY(date)',
        parameters: [
          { name: 'date', description: 'The date to extract day from', required: true }
        ],
        examples: [
          { formula: 'DAY(DATE(2024, 1, 15))', result: '15', description: 'Extract day from date' },
          { formula: 'DAY(TODAY())', result: '15', description: 'Current day' }
        ]
      },
      WEEKDAY: {
        name: 'WEEKDAY',
        description: 'Returns the day of the week for a date',
        syntax: 'WEEKDAY(date, [return_type])',
        parameters: [
          { name: 'date', description: 'The date to get weekday for', required: true },
          { name: 'return_type', description: 'Format for weekday (1=Sun-Sat 1-7, 2=Mon-Sun 1-7)', required: false }
        ],
        examples: [
          { formula: 'WEEKDAY(DATE(2024, 1, 15))', result: '2', description: 'Monday = 2 (Sunday=1 system)' },
          { formula: 'WEEKDAY(TODAY(), 2)', result: '1', description: 'Monday = 1 (Monday=1 system)' }
        ]
      },
      HOUR: {
        name: 'HOUR',
        description: 'Returns the hour of a time value',
        syntax: 'HOUR(time)',
        parameters: [
          { name: 'time', description: 'The time to extract hour from', required: true }
        ],
        examples: [
          { formula: 'HOUR(TIME(14, 30, 0))', result: '14', description: 'Extract hour from time' },
          { formula: 'HOUR(NOW())', result: '14', description: 'Current hour' }
        ]
      },
      MINUTE: {
        name: 'MINUTE',
        description: 'Returns the minute of a time value',
        syntax: 'MINUTE(time)',
        parameters: [
          { name: 'time', description: 'The time to extract minute from', required: true }
        ],
        examples: [
          { formula: 'MINUTE(TIME(14, 30, 45))', result: '30', description: 'Extract minute from time' },
          { formula: 'MINUTE(NOW())', result: '30', description: 'Current minute' }
        ]
      },
      SECOND: {
        name: 'SECOND',
        description: 'Returns the second of a time value',
        syntax: 'SECOND(time)',
        parameters: [
          { name: 'time', description: 'The time to extract second from', required: true }
        ],
        examples: [
          { formula: 'SECOND(TIME(14, 30, 45))', result: '45', description: 'Extract second from time' },
          { formula: 'SECOND(NOW())', result: '23', description: 'Current second' }
        ]
      },
      TIME: {
        name: 'TIME',
        description: 'Creates a time value from hour, minute, and second',
        syntax: 'TIME(hour, minute, second)',
        parameters: [
          { name: 'hour', description: 'Hour value (0-23)', required: true },
          { name: 'minute', description: 'Minute value (0-59)', required: true },
          { name: 'second', description: 'Second value (0-59)', required: true }
        ],
        examples: [
          { formula: 'TIME(14, 30, 0)', result: '0.604167', description: 'Create time 2:30 PM' },
          { formula: 'TIME(12, 0, 0)', result: '0.5', description: 'Create noon time' }
        ]
      },
      DATE: {
        name: 'DATE',
        description: 'Creates a date value from year, month, and day',
        syntax: 'DATE(year, month, day)',
        parameters: [
          { name: 'year', description: 'Year value', required: true },
          { name: 'month', description: 'Month value (1-12)', required: true },
          { name: 'day', description: 'Day value (1-31)', required: true }
        ],
        examples: [
          { formula: 'DATE(2024, 1, 15)', result: '45306', description: 'Create date January 15, 2024' },
          { formula: 'DATE(2025, 12, 25)', result: '46065', description: 'Create Christmas 2025' }
        ]
      },
      DATEDIF: {
        name: 'DATEDIF',
        description: 'Calculates the difference between two dates',
        syntax: 'DATEDIF(start_date, end_date, unit)',
        parameters: [
          { name: 'start_date', description: 'The starting date', required: true },
          { name: 'end_date', description: 'The ending date', required: true },
          { name: 'unit', description: 'Unit of time ("Y", "M", "D", "YM", "YD", "MD")', required: true }
        ],
        examples: [
          { formula: 'DATEDIF(DATE(2020,1,1), DATE(2024,1,1), "Y")', result: '4', description: 'Years between dates' },
          { formula: 'DATEDIF(DATE(2024,1,1), DATE(2024,3,15), "D")', result: '74', description: 'Days between dates' }
        ]
      }
    }
  }
}

export const getAllFunctions = () => {
  const allFunctions = []
  Object.entries(functionCategories).forEach(([categoryKey, category]) => {
    if (categoryKey === 'all') return // Skip the "all" category to avoid infinite loop
    Object.entries(category.functions).forEach(([functionKey, func]) => {
      allFunctions.push({
        ...func,
        category: categoryKey,
        categoryName: category.name
      })
    })
  })
  return allFunctions
}

// Populate the "all" category with all functions
export const populateAllCategory = () => {
  const allFunctions = getAllFunctions()
  const allFunctionsMap = {}
  allFunctions.forEach(func => {
    allFunctionsMap[func.name] = func
  })
  functionCategories.all.functions = allFunctionsMap
  return functionCategories
}

// Initialize the all category
populateAllCategory()