# 필요한 라이브러리 임포트
from flask import Flask, request, jsonify  # Flask 웹 서버, 요청 처리, JSON 응답
from flask_cors import CORS                # CORS 설정 (다른 포트/도메인에서 접근 허용)
import os                                  # 파일 존재 여부 확인용
import json                                # JSON 파일 입출력

# Flask 애플리케이션 생성
app = Flask(__name__)
CORS(app)  # 모든 출처에서 오는 요청 허용 (CORS 에러 방지)

# 저장할 JSON 파일 경로
DATA_FILE = "tasks.json"

# 할 일 목록을 가져오는 API
@app.route("/tasks", methods=["GET"])
def get_tasks():
    # 파일이 존재하면 열어서 JSON으로 읽어오기
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE, "r", encoding="utf-8") as f:
            return jsonify(json.load(f))  # 읽은 데이터를 JSON 응답으로 반환
    return jsonify([])  # 파일이 없으면 빈 배열 반환

# 할 일 목록을 저장하는 API
@app.route("/tasks", methods=["POST"])
def save_tasks():
    tasks = request.json  # 클라이언트에서 보낸 JSON 데이터 받기
    with open(DATA_FILE, "w", encoding="utf-8") as f:
        json.dump(tasks, f, ensure_ascii=False, indent=2)  # 파일에 저장
    return jsonify({"message": "저장 완료!"})  # 저장 완료 메시지 반환

# 서버 실행
if __name__ == "__main__":
    app.run(debug=True, port=3001)  # 3001번 포트에서 디버그 모드로 실행
