from flask import (
    Flask,
    Request
)

app = Flask(__name__)

@app.route("/", methods=["GET"])
def Register():
    return "OK"

app.run("0.0.0.0", 8000, ssl_context=("Certs/server.crt", "Certs/server.key"))
        
