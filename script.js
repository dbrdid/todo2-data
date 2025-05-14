let tasks = []; // 할 일 목록을 저장할 배열

// 페이지가 로드될 때 서버에서 할 일 목록을 불러와서 렌더링
window.onload = function () {
  fetch("http://localhost:3001/tasks") // 서버에서 할 일 목록 데이터 받아오기
    .then(res => res.json()) // JSON 형태로 응답을 변환
    .then(data => {
      tasks = data; // 받은 데이터(tasks)를 클라이언트의 tasks 배열에 저장
      renderTasks(); // 받은 데이터를 화면에 렌더링
    })
    .catch(err => console.error("서버에서 데이터를 불러오는 데 실패했습니다.", err)); // 오류 처리
};

// 할 일 목록을 서버에 저장하는 함수
function saveTasks() {
  fetch("http://localhost:3001/tasks", {
    method: "POST", // POST 방식으로 데이터를 서버에 전송
    headers: { "Content-Type": "application/json" }, // 서버가 JSON 형식을 이해하도록 헤더 설정
    body: JSON.stringify(tasks), // 클라이언트에서 수정된 tasks 배열을 JSON 문자열로 변환하여 전송
  })
  .then(res => res.json()) // 서버 응답 처리
  .then(response => console.log(response.message)) // 서버 응답 메시지 출력
  .catch(err => console.error("서버에 데이터를 저장하는 데 실패했습니다.", err)); // 오류 처리
}

// 새 할 일을 추가하는 함수
function addTask() {
  const desc = document.getElementById("task").value.trim(); // 할 일 입력값 가져오기
  const cat = document.getElementById("category").value.trim(); // 카테고리 입력값 가져오기

  // 할 일과 카테고리가 모두 입력되지 않으면 경고 메시지 표시
  if (!desc || !cat) return alert("할 일과 카테고리를 입력하세요!");

  // 새로운 할 일 객체 생성
  const newTask = {
    id: Date.now(), // 고유 id 생성 (현재 시간)
    description: desc, // 할 일 내용
    category: cat, // 카테고리
    completed: false, // 완료 여부 (기본값: false)
  };

  tasks.push(newTask); // 새로운 할 일 추가
  saveTasks(); // 서버에 저장
  renderTasks(); // 화면 갱신

  // 입력 필드 초기화
  document.getElementById("task").value = "";
  document.getElementById("category").value = "";
}

// 할 일 목록을 화면에 렌더링하는 함수
function renderTasks() {
  const list = document.getElementById("taskList");
  list.innerHTML = "";

  tasks.forEach((task) => {
    const li = document.createElement("li");
    if (task.completed) li.classList.add("completed");

    li.innerHTML = `
      <span class="text">[${task.category}] ${task.description}</span>
      <div class="buttons">
        <button onclick="toggle(${task.id})">완료</button>
        <button onclick="remove(${task.id})">삭제</button>
      </div>
    `;

    list.appendChild(li);
  });
}


// 할 일의 완료 상태를 토글하는 함수
function toggle(id) {
  const t = tasks.find((t) => t.id === id); // 해당 id의 할 일 찾기
  if (t) t.completed = !t.completed; // 완료 상태 반전 (false -> true / true -> false)
  saveTasks(); // 서버에 저장
  renderTasks(); // 화면 갱신
}

// 할 일을 목록에서 삭제하는 함수
function remove(id) {
  tasks = tasks.filter((t) => t.id !== id); // 해당 id의 할 일 삭제
  saveTasks(); // 서버에 저장
  renderTasks(); // 화면 갱신
}
