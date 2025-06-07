function func() {
  var var_num = document.getElementById('var_num').value;
  var lim_num = document.getElementById('lim_num').value;

  let container = document.getElementById('inputContainer');
  while (container.hasChildNodes()) {
    container.removeChild(container.lastChild);
  }
  for (let i = 0; i < var_num; i++) {
    let input = document.createElement('input');
    input.type = 'number';
    input.id = `dynamicInput${i}`;
    input.style = "width:80px";
    input.value = '0';
    const label = document.createElement('span');
    label.className = 'field-label';
    if (i < var_num - 1) {
      label.textContent = `x${i + 1}+`;
    } 
    else {
      label.textContent = `x${i + 1}→`;
    }
    container.appendChild(input);
    container.appendChild(label);
  }

  const select = document.createElement('select');
  select.id = 'maxmin';
  select.name = 'maxmin';
  const options = ['max', 'min'];
  options.forEach((optionText, index) => {
    const option = document.createElement('option');
    option.value = `option${index + 1}`;
    option.textContent = optionText;
    select.appendChild(option);
  });    
  container.appendChild(select);
}

function ogran() {
  var var_num = document.getElementById('var_num').value;
  var lim_num = document.getElementById('lim_num').value;

  let container = document.getElementById('inputContainer2');
  while (container.hasChildNodes()) {
    container.removeChild(container.lastChild);
  }
  for (let i = 0; i < lim_num; i++) {
    for (let j = 0; j < var_num; j++) {
      let input = document.createElement('input');
      input.type = 'number';
      input.id = `dynamicInput${i}${j}`;
      input.style = "width:80px";
      input.value = '0';
      
      const label = document.createElement('span');
      label.className = 'field-label';
      if (j < var_num - 1) {
        label.textContent = `x${j + 1}+`;
      } 
      else {
        label.textContent = `x${j + 1}`;
      }
      container.appendChild(input);
      container.appendChild(label);
    }

    const select = document.createElement('select');
    select.id = `options${i}`;
    select.name = `options${i}`;
    const options = ['≤', '=','≥'];
    options.forEach((optionText, index) => {
      const option = document.createElement('option');
      option.value = `option${index + 1}`;
      option.textContent = optionText;
      select.appendChild(option);
    });

    let input2 = document.createElement('input');
    input2.type = 'number';
    input2.id = `dynamicOtvet${i}`;
    input2.style = "width:80px";
    input2.value = '0';
    
    container.appendChild(select);
    container.appendChild(input2);
    container.appendChild(document.createElement('br'))
  }
}

function SimplexMethod() {
  let container = document.getElementById('outputContainer1');
  while (container.hasChildNodes()) {
    container.removeChild(container.lastChild);
  }

  // Получаем данные
  let var_num = parseInt(document.getElementById('var_num').value);
  let var_num_dop = var_num;
  let lim_num = parseInt(document.getElementById('lim_num').value);
  let isMax = document.getElementById('maxmin').value === 'option1';

  // Заголовок
  let v1 = document.createElement('label');
  v1.textContent = 'Приводим ограничения к каноническому виду:';
  container.appendChild(v1);
  container.appendChild(document.createElement('br'));

  // Формируем матрицу с целевой функцией
  const targetFunc = [];
  for (let i = 0; i < var_num; i++) {
    targetFunc.push(parseFloat(document.getElementById(`dynamicInput${i}`).value));
  }

  // Считаем количество дополнительных переменных
  let num_dop = 0;
  for (let i = 0; i < lim_num; i++) {
    let sign = document.getElementById(`options${i}`).value;
    if (sign != 'option2') {
      num_dop++;
    }
  }
  var_num_dop += num_dop;

  // Формируем матрицу ограничений
  const matrix = [];
  const basis = [];
  let slackIndex = var_num;
  for (let i = 0; i < lim_num; i++) {
    matrix[i] = new Array(var_num_dop + 1).fill(0);
    let sign = document.getElementById(`options${i}`).value;
    let b = parseFloat(document.getElementById(`dynamicOtvet${i}`).value);

    // Заполняем коэффициенты переменных
    for (let j = 0; j < var_num; j++) {
      matrix[i][j] = parseFloat(document.getElementById(`dynamicInput${i}${j}`).value);
    }

    if (sign === 'option1') {
      matrix[i][slackIndex] = 1;
      basis.push(slackIndex);
      slackIndex++;
    } 
    else {
      if (sign === 'option3') {
        matrix[i][slackIndex] = -1;
        slackIndex++;
      }
    }

    // Правая часть уравнения
    matrix[i][var_num_dop] = b;

    // Выводим уравнение
    let equation = '';
    for (let j = 0; j < var_num; j++) {
      if (j > 0 && matrix[i][j] >= 0) {
        equation += ' + ';
      }
      equation += matrix[i][j] + 'x' + (j+1);
    }
    
    if (sign === 'option1') {
      equation += ' + x' + (slackIndex) + ' = ';
    } 
    else {
      if (sign === 'option3') {
        equation += ' - x' + (slackIndex) + ' = ';
      } 
      else {
        equation += ' = ';
      }
    }
    equation += b;

    let eqLabel = document.createElement('label');
    eqLabel.textContent = equation;
    container.appendChild(eqLabel);
    container.appendChild(document.createElement('br'));
  }

  // Создаем симплекс-таблицу
  let simplexTable = [];
  const numVars = var_num_dop;
  const numConstraints = lim_num;

  // Инициализируем таблицу
  for (let i = 0; i <= numConstraints; i++) {
    simplexTable[i] = new Array(numVars + 1).fill(0);
  }

  // Заполняем целевую функцию
  for (let j = 0; j < var_num; j++) {
    if (isMax) {
      simplexTable[0][j] = -targetFunc[j];
    } 
    else {
      simplexTable[0][j] = targetFunc[j];
    }
  }

  // Заполняем ограничения
  for (let i = 0; i < numConstraints; i++) {
    for (let j = 0; j <= numVars; j++) {
      simplexTable[i+1][j] = matrix[i][j];
    }
  }

  // Выводим начальную таблицу
  displayTable(container, simplexTable, basis, 'Начальная симплекс-таблица:');

  // Основной цикл симплекс-метода
  let iteration = 1;
  while (true) {
    // Шаг 1: Проверка на оптимальность
    let optimal = true;
    let pivotCol = -1;
    for (let j = 0; j < numVars; j++) {
      if (simplexTable[0][j] < 0) {
        optimal = false;
        pivotCol = j;
        break;
      }
    }

    if (optimal) break;

    // Шаг 2: Выбор ведущего столбца
    if (pivotCol === -1) {
      pivotCol = 0;
      for (let j = 1; j < numVars; j++) {
        if (simplexTable[0][j] < simplexTable[0][pivotCol]) {
          pivotCol = j;
        }
      }
    }

    // Шаг 3: Выбор ведущей строки
    let pivotRow = -1;
    let minRatio = Infinity;
    for (let i = 1; i <= numConstraints; i++) {
      if (simplexTable[i][pivotCol] > 0) {
        let ratio = simplexTable[i][numVars] / simplexTable[i][pivotCol];
        if (ratio < minRatio) {
          minRatio = ratio;
          pivotRow = i;
        }
      }
    }

    if (pivotRow === -1) {
      alert('Задача не имеет решения!');
      return;
    }

    // Обновляем базис
    basis[pivotRow-1] = pivotCol;

    // Шаг 4: Пересчет таблицы
    let pivotElement = simplexTable[pivotRow][pivotCol];
    for (let j = 0; j <= numVars; j++) {
      simplexTable[pivotRow][j] /= pivotElement;
    }

    for (let i = 0; i <= numConstraints; i++) {
      if (i !== pivotRow) {
        let factor = simplexTable[i][pivotCol];
        for (let j = 0; j <= numVars; j++) {
          simplexTable[i][j] -= simplexTable[pivotRow][j] * factor;
        }
      }
    }

    // Выводим таблицу после итерации
    displayTable(container, simplexTable, basis, `Итерация ${iteration}:`);
    iteration++;
  }

  // Выводим результат
  let resultLabel = document.createElement('h4');
  resultLabel.textContent = 'Результат:';
  container.appendChild(resultLabel);

  let solution = new Array(numVars).fill(0);
  for (let i = 0; i < basis.length; i++) {
    solution[basis[i]] = simplexTable[i+1][numVars];
  }

  for (let i = 0; i < var_num; i++) {
    let solLabel = document.createElement('label');
    solLabel.textContent = `x${i+1} = ${solution[i].toFixed(2)}`;
    container.appendChild(solLabel);
    container.appendChild(document.createElement('br'));
  }

  let zLabel = document.createElement('label');
  let zValue;
  if (isMax) {
    zValue = simplexTable[0][numVars];
  } 
  else {
    zValue = -simplexTable[0][numVars];
  }
  zLabel.textContent = `Целевая функция: ${zValue.toFixed(2)}`;
  container.appendChild(zLabel);
}

function displayTable(container, table, basis, title) {
  let titleLabel = document.createElement('h4');
  titleLabel.textContent = title;
  container.appendChild(titleLabel);

  let tableElem = document.createElement('table');
  tableElem.className = 'table table-bordered';
  let thead = document.createElement('thead');
  let tbody = document.createElement('tbody');

  let headerRow = document.createElement('tr');
  let emptyHeader = document.createElement('th');
  headerRow.appendChild(emptyHeader);
  
  for (let j = 0; j < table[0].length - 1; j++) {
    let th = document.createElement('th');
    th.textContent = `x${j+1}`;
    headerRow.appendChild(th);
  }
  
  let bHeader = document.createElement('th');
  bHeader.textContent = 'b';
  headerRow.appendChild(bHeader);
  thead.appendChild(headerRow);
  tableElem.appendChild(thead);

  let CRow = document.createElement('tr');
  let CLabel = document.createElement('td');
  CLabel.textContent = 'C';
  CRow.appendChild(CLabel);
  
  for (let j = 0; j < table[0].length; j++) {
    let td = document.createElement('td');
    td.textContent = table[0][j].toFixed(2);
    CRow.appendChild(td);
  }
  tbody.appendChild(CRow);

  for (let i = 1; i < table.length; i++) {
    let row = document.createElement('tr');
    let basisLabel = document.createElement('td');
    basisLabel.textContent = `x${basis[i-1]+1}`;
    row.appendChild(basisLabel);
    
    for (let j = 0; j < table[i].length; j++) {
      let td = document.createElement('td');
      td.textContent = table[i][j].toFixed(2);
      row.appendChild(td);
    }
    tbody.appendChild(row);
  }

  tableElem.appendChild(tbody);
  container.appendChild(tableElem);
  container.appendChild(document.createElement('br'));
}