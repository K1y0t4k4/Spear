from flask import (
    Flask,
    request
)

app = Flask(__name__)

@app.route("/recv", methods=["GET"])
def Register():
    print("------GET------")
    print(request.headers.get("Host-Name"))
    print(request.headers.get("OS"))
    print(request.headers.get("Cwd"))
    return "{\"status\": \"OK\"}"

@app.route("/send", methods=["POST"])
def Recv():
    print("------POST------")
    print(request.get_json()["data"])
    return "{\"status\": \"OK\"}"

app.run("0.0.0.0", 8000, ssl_context=("Certs/server.crt", "Certs/server.key"))
        
