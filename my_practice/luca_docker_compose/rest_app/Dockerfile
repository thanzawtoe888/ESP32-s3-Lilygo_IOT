FROM python:3.12

WORKDIR /python-docker
COPY requirements.txt ./
RUN pip install -r requirements.txt
COPY . .
EXPOSE 5000
CMD ["gunicorn", "app:app", "--bind", "0.0.0.0:5000"] #gunicorn is a web server environements