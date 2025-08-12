describe('XL Formula Integration Tests', () => {
  let XLFormulaModule;
  let engine;

  // Helper function to format evaluation results consistently
  const formatResult = (result) => {
    if (!result.isSuccess()) {
      throw new Error(result.getErrorMessage());
    }
    const value = result.getValue();
    if (value.isNumber()) {
      const num = value.asNumber();
      return Number.isInteger(num) ? num.toString() : num.toFixed(6).replace(/\.?0+$/, '');
    } else if (value.isBoolean()) {
      return value.asBoolean() ? 'TRUE' : 'FALSE';
    } else if (value.isError()) {
      return value.getErrorText();
    }
    return value.asText();
  };

  beforeAll(async () => {
    // Wait for global WASM initialization to complete
    await global.wasmInitializationPromise;
    
    if (!global.hasWasmAvailable()) {
      throw new Error('WASM not available');
    }

    // Get the globally initialized instances
    XLFormulaModule = global.getXLFormulaModule();
    engine = global.getFormulaEngine();
  });

  describe('Basic Functionality', () => {
    test('should initialize without errors', () => {
      expect(XLFormulaModule).toBeDefined();
      expect(engine).toBeDefined();
      expect(typeof engine.evaluate).toBe('function');
    });

    test('evaluate with per-call variables', () => {
      const result = engine.evaluate('X + Y + 1', { X: 10, Y: 5 });
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(16);
    });
  });

  describe('Math Functions', () => {
    test('SUM() should add numbers correctly', () => {
      const result = engine.evaluate('SUM(1, 2, 3)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(6);
    });

    test('MAX() should find maximum correctly', () => {
      const result = engine.evaluate('MAX(10, 20, 5)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(20);
    });

    test('AVERAGE() should calculate average correctly', () => {
      const result = engine.evaluate('AVERAGE(1, 2, 3, 4, 5)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(3);
    });

    test('SUMSQ() should return sum of squares correctly', () => {
      const result = engine.evaluate('SUMSQ(2, 3, 4)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(29); // 2^2 + 3^2 + 4^2 = 4 + 9 + 16 = 29
    });

    test('QUOTIENT() should return integer division correctly', () => {
      const result = engine.evaluate('QUOTIENT(10, 3)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(3); // 10/3 = 3.333..., truncated to 3
    });

    test('EVEN() should round to nearest even integer', () => {
      const result1 = engine.evaluate('EVEN(1.5)');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(2);

      const result2 = engine.evaluate('EVEN(-1.5)');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(-2);
    });

    test('ODD() should round to nearest odd integer', () => {
      const result1 = engine.evaluate('ODD(2)');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(3);

      const result2 = engine.evaluate('ODD(0)');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(1);
    });

    test('MROUND() should round to nearest multiple', () => {
      const result = engine.evaluate('MROUND(10, 3)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(9); // Nearest multiple of 3 to 10 is 9
    });

    test('ROUNDUP() should round away from zero', () => {
      const result1 = engine.evaluate('ROUNDUP(3.2, 0)');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(4);

      const result2 = engine.evaluate('ROUNDUP(-3.2, 0)');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(-4);
    });

    test('ROUNDDOWN() should round toward zero', () => {
      const result1 = engine.evaluate('ROUNDDOWN(3.9, 0)');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(3);

      const result2 = engine.evaluate('ROUNDDOWN(-3.9, 0)');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(-3);
    });
  });

  describe('DateTime Functions - Critical Test', () => {
    test('NOW() should return a valid date string', () => {
      const result = engine.evaluate('NOW()');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isDate()).toBe(true);
      const formatted = value.asText();
      expect(formatted).toMatch(/^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$/);
    });

    test('TODAY() should return a valid date string', () => {
      const result = engine.evaluate('TODAY()');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isDate()).toBe(true);
      const formatted = value.asText();
      expect(formatted).toMatch(/^\d{4}-\d{2}-\d{2} 00:00:00$/);
    });

    test('DATE() should create valid dates', () => {
      const result = engine.evaluate('DATE(2024, 1, 15)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isDate()).toBe(true);
      const formatted = value.asText();
      expect(formatted).toMatch(/^\d{4}-\d{2}-\d{2} 00:00:00$/);
    });

    test('YEAR() should extract year correctly', () => {
      const result = engine.evaluate('YEAR(DATE(2024, 1, 15))');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(2024);
    });

    test('MONTH() should extract month correctly', () => {
      const result = engine.evaluate('MONTH(DATE(2024, 3, 15))');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(3);
    });

    test('DAY() should extract day correctly', () => {
      const result = engine.evaluate('DAY(DATE(2024, 1, 15))');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(15);
    });

    test('NOW() should return a JavaScript Date object', () => {
      const evalResult = engine.evaluate('NOW()');
      expect(evalResult.isSuccess()).toBe(true);
      const value = evalResult.getValue();
      expect(value.isDate()).toBe(true);
      
      const result = value.asDate();
      console.log('🔍 NOW() as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getTime()).toBeGreaterThan(0);
    });

    test('TODAY() should return a JavaScript Date object', () => {
      const evalResult = engine.evaluate('TODAY()');
      expect(evalResult.isSuccess()).toBe(true);
      const value = evalResult.getValue();
      expect(value.isDate()).toBe(true);
      
      const result = value.asDate();
      console.log('🔍 TODAY() as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getTime()).toBeGreaterThan(0);
      // Should be midnight (00:00:00)
      expect(result.getHours()).toBe(0);
      expect(result.getMinutes()).toBe(0);
      expect(result.getSeconds()).toBe(0);
    });

    test('DATE() should return a JavaScript Date object', () => {
      const evalResult = engine.evaluate('DATE(2024, 1, 15)');
      expect(evalResult.isSuccess()).toBe(true);
      const value = evalResult.getValue();
      expect(value.isDate()).toBe(true);
      
      const result = value.asDate();
      console.log('🔍 DATE(2024, 1, 15) as Date:', result, 'Type:', typeof result);
      
      expect(result).toBeInstanceOf(Date);
      expect(result.getFullYear()).toBe(2024);
      expect(result.getMonth()).toBe(0); // January is 0
      expect(result.getDate()).toBe(15);
      // Should be midnight (00:00:00)
      expect(result.getHours()).toBe(0);
      expect(result.getMinutes()).toBe(0);
      expect(result.getSeconds()).toBe(0);
    });
  });

  describe('Text Functions', () => {
    test('CONCATENATE() should join strings correctly', () => {
      const result = engine.evaluate('CONCATENATE("Hello", " ", "World")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('Hello World');
    });

    test('LEN() should return correct string length', () => {
      const result = engine.evaluate('LEN("Hello World")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(11);
    });

    test('CHAR() should convert number to character', () => {
      const result = engine.evaluate('CHAR(65)');
      expect(result.isSuccess()).toBe(true);
      
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('A');
    });

    test('CODE() should convert character to number', () => {
      const result = engine.evaluate('CODE("A")');
      expect(result.isSuccess()).toBe(true);
      
      const value = result.getValue();
      expect(value.isNumber()).toBe(true);
      expect(value.asNumber()).toBe(65);
    });

    test('CLEAN() should remove non-printable characters', () => {
      const result = engine.evaluate('CLEAN("Hello\tWorld\n")');
      expect(result.isSuccess()).toBe(true);
      
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('HelloWorld');
    });

    test('EXACT() should compare strings exactly', () => {
      const result1 = engine.evaluate('EXACT("Hello", "Hello")');
      expect(result1.isSuccess()).toBe(true);
      expect(result1.getValue().isBoolean()).toBe(true);
      expect(result1.getValue().asBoolean()).toBe(true);

      const result2 = engine.evaluate('EXACT("Hello", "hello")');
      expect(result2.isSuccess()).toBe(true);
      expect(result2.getValue().isBoolean()).toBe(true);
      expect(result2.getValue().asBoolean()).toBe(false);
    });

    test('CONCAT() should work as alias for CONCATENATE', () => {
      const result = engine.evaluate('CONCAT("Hello", " ", "World")');
      expect(result.isSuccess()).toBe(true);
      expect(result.getValue().isText()).toBe(true);
      expect(result.getValue().asText()).toBe('Hello World');
    });

    test('T() should coerce values to text', () => {
      // Text input should return text unchanged
      const result1 = engine.evaluate('T("Hello")');
      expect(result1.isSuccess()).toBe(true);
      expect(result1.getValue().isText()).toBe(true);
      expect(result1.getValue().asText()).toBe('Hello');
      
      // Number input should return empty string
      const result2 = engine.evaluate('T(123)');
      expect(result2.isSuccess()).toBe(true);
      expect(result2.getValue().isText()).toBe(true);
      expect(result2.getValue().asText()).toBe('');
    });

    test('TEXTJOIN() should join text with delimiter', () => {
      const result = engine.evaluate('TEXTJOIN(",", TRUE, "A", "", "B", "C")');
      expect(result.isSuccess()).toBe(true);
      expect(result.getValue().isText()).toBe(true);
      expect(result.getValue().asText()).toBe('A,B,C'); // Empty string ignored
    });

    test('UNICHAR() should convert code to Unicode character', () => {
      const result = engine.evaluate('UNICHAR(65)');
      expect(result.isSuccess()).toBe(true);
      expect(result.getValue().isText()).toBe(true);
      expect(result.getValue().asText()).toBe('A');
    });

    test('UNICODE() should convert character to code', () => {
      const result = engine.evaluate('UNICODE("A")');
      expect(result.isSuccess()).toBe(true);
      expect(result.getValue().isNumber()).toBe(true);
      expect(result.getValue().asNumber()).toBe(65);
    });
  });

  describe('Logical Functions', () => {
    test('IF() should handle conditional logic correctly', () => {
      const result1 = engine.evaluate('IF(TRUE, "Yes", "No")');
      const result2 = engine.evaluate('IF(FALSE, "Yes", "No")');
      expect(result1.isSuccess()).toBe(true);
      expect(result2.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      const value2 = result2.getValue();
      expect(value1.isText()).toBe(true);
      expect(value2.isText()).toBe(true);
      expect(value1.asText()).toBe('Yes');
      expect(value2.asText()).toBe('No');
    });

    test('AND() should handle logical AND correctly', () => {
      const result1 = engine.evaluate('AND(TRUE, TRUE)');
      const result2 = engine.evaluate('AND(TRUE, FALSE)');
      expect(result1.isSuccess()).toBe(true);
      expect(result2.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      const value2 = result2.getValue();
      expect(value1.isBoolean()).toBe(true);
      expect(value2.isBoolean()).toBe(true);
      expect(value1.asBoolean()).toBe(true);
      expect(value2.asBoolean()).toBe(false);
    });

    test('SWITCH() should handle multiple conditions correctly', () => {
      const result1 = engine.evaluate('SWITCH(2, 1, "One", 2, "Two", 3, "Three")');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isText()).toBe(true);
      expect(value1.asText()).toBe('Two');
      
      // Test with default value
      const result2 = engine.evaluate('SWITCH(4, 1, "One", 2, "Two", "Default")');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isText()).toBe(true);
      expect(value2.asText()).toBe('Default');
    });

    test('IFS() should handle multiple IF conditions correctly', () => {
      const result1 = engine.evaluate('IFS(FALSE, "First", TRUE, "Second", TRUE, "Third")');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isText()).toBe(true);
      expect(value1.asText()).toBe('Second');
      
      // Test with numeric conditions
      const result2 = engine.evaluate('IFS(0, "Zero", 1, "One")');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isText()).toBe(true);
      expect(value2.asText()).toBe('One');
    });
  });

  describe('Engineering Functions', () => {
    test('DEC2OCT() should convert decimal to octal correctly', () => {
      const result = engine.evaluate('DEC2OCT(8)');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('10');
    });

    test('BIN2OCT() should convert binary to octal correctly', () => {
      const result = engine.evaluate('BIN2OCT("1000")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('10');
    });

    test('OCT2BIN() should convert octal to binary correctly', () => {
      const result = engine.evaluate('OCT2BIN("10")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('1000');
    });

    test('HEX2OCT() should convert hexadecimal to octal correctly', () => {
      const result = engine.evaluate('HEX2OCT("A")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('12');
    });

    test('OCT2HEX() should convert octal to hexadecimal correctly', () => {
      const result = engine.evaluate('OCT2HEX("12")');
      expect(result.isSuccess()).toBe(true);
      const value = result.getValue();
      expect(value.isText()).toBe(true);
      expect(value.asText()).toBe('A');
    });

    test('COMPLEX() should create complex numbers correctly', () => {
      const result1 = engine.evaluate('COMPLEX(3, 4)');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isText()).toBe(true);
      expect(value1.asText()).toBe('3+4i');

      const result2 = engine.evaluate('COMPLEX(5, -2)');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isText()).toBe(true);
      expect(value2.asText()).toBe('5-2i');

      const result3 = engine.evaluate('COMPLEX(0, 1)');
      expect(result3.isSuccess()).toBe(true);
      const value3 = result3.getValue();
      expect(value3.isText()).toBe(true);
      expect(value3.asText()).toBe('i');
    });

    test('IMREAL() should extract real part correctly', () => {
      const result1 = engine.evaluate('IMREAL("3+4i")');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(3);

      const result2 = engine.evaluate('IMREAL("7")');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(7);

      const result3 = engine.evaluate('IMREAL("3i")');
      expect(result3.isSuccess()).toBe(true);
      const value3 = result3.getValue();
      expect(value3.isNumber()).toBe(true);
      expect(value3.asNumber()).toBe(0);
    });

    test('IMAGINARY() should extract imaginary part correctly', () => {
      const result1 = engine.evaluate('IMAGINARY("3+4i")');
      expect(result1.isSuccess()).toBe(true);
      const value1 = result1.getValue();
      expect(value1.isNumber()).toBe(true);
      expect(value1.asNumber()).toBe(4);

      const result2 = engine.evaluate('IMAGINARY("7")');
      expect(result2.isSuccess()).toBe(true);
      const value2 = result2.getValue();
      expect(value2.isNumber()).toBe(true);
      expect(value2.asNumber()).toBe(0);

      const result3 = engine.evaluate('IMAGINARY("3i")');
      expect(result3.isSuccess()).toBe(true);
      const value3 = result3.getValue();
      expect(value3.isNumber()).toBe(true);
      expect(value3.asNumber()).toBe(3);
    });
  });
});