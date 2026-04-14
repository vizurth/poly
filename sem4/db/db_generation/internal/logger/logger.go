package logger

import (
	"context"
	"fmt"

	"go.uber.org/zap"
)

type key string

const (
	KeyForLogger    key = "logger"
	KeyForRequestID key = "requestID"
)

type Logger struct {
	l *zap.Logger
}

func New(ctx context.Context) (context.Context, error) {
	l, err := zap.NewProduction()
	if err != nil {
		return ctx, fmt.Errorf("failed to create logger: %w", err)
	}
	wrapped := &Logger{l: l}
	ctx = context.WithValue(ctx, KeyForLogger, wrapped)
	return ctx, nil
}

func GetLoggerFromCtx(ctx context.Context) (*Logger, error) {
	if l, ok := ctx.Value(KeyForLogger).(*Logger); ok {
		return l, nil
	}
	return nil, fmt.Errorf("logger not found in context")
}

func (l *Logger) Debug(ctx context.Context, msg string, fields ...zap.Field) {
	if ctx.Value(KeyForRequestID) != nil {
		fields = append(fields, zap.String(string(KeyForRequestID), ctx.Value(KeyForRequestID).(string)))
	}
	l.l.Debug(msg, fields...)
}

func (l *Logger) Info(ctx context.Context, msg string, fields ...zap.Field) {
	if ctx.Value(KeyForRequestID) != nil {
		fields = append(fields, zap.String(string(KeyForRequestID), ctx.Value(KeyForRequestID).(string)))
	}
	l.l.Info(msg, fields...)
}

func (l *Logger) Warn(ctx context.Context, msg string, fields ...zap.Field) {
	if ctx.Value(KeyForRequestID) != nil {
		fields = append(fields, zap.String(string(KeyForRequestID), ctx.Value(KeyForRequestID).(string)))
	}
	l.l.Warn(msg, fields...)
}

func (l *Logger) Error(ctx context.Context, msg string, fields ...zap.Field) {
	if ctx.Value(KeyForRequestID) != nil {
		fields = append(fields, zap.String(string(KeyForRequestID), ctx.Value(KeyForRequestID).(string)))
	}
	l.l.Error(msg, fields...)
}
func (l *Logger) Fatal(ctx context.Context, msg string, fields ...zap.Field) {
	if ctx.Value(KeyForRequestID) != nil {
		fields = append(fields, zap.String(string(KeyForRequestID), ctx.Value(KeyForRequestID).(string)))
	}
	l.l.Fatal(msg, fields...)
}
