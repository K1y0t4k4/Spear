from flask import (
    Flask,
    request
)

app = Flask(__name__)

@app.route("/", methods=["GET"])
def Register():
    print("------GET------")
    print(request.headers)
    return "{\"status\": \"OK\"}"

@app.route("/send", methods=["POST"])
def Recv():
    print("------POST------")
    print(request.get_json())

app.run("0.0.0.0", 8000, ssl_context=("Certs/server.crt", "Certs/server.key"))
        
